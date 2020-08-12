package ch.broox.ble;

import ch.broox.ble.utils.SerialCommunicator;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class BLEDriver implements AutoCloseable {
    private static String TOKEN_DELIMITER = " ";
    private static String LINE_DELIMITER = "\n";

    private final SerialCommunicator communicator = new SerialCommunicator();
    private volatile boolean open = false;

    public boolean open(String port, int baudRate) {
        if (open) return false;
        open = true;
        return communicator.open(port, baudRate);
    }

    public void close() {
        if (!open) return;
        communicator.close();
        open = false;
    }

    public List<BLEDevice> scan(int interval, int window, int time, String serviceAddress) {
        String result = communicator.sendCommandAndAwait(String.format("scan %s %s %s %s", interval, window, time, serviceAddress));
        String[] lines = result.split(LINE_DELIMITER);

        return Arrays.stream(lines).map(line -> {
                    String[] tokens = line.split(TOKEN_DELIMITER);
                    return new BLEDevice(this, tokens[0], (tokens.length > 1) ? tokens[1] : "");
                }).collect(Collectors.toList());
    }

    public void connect(String deviceId) {

    }

    public void disconnect(String deviceId) {

    }

    public List<BLEDevice> list() {
        return null;
    }

    public String read() {
        return null;
    }

    public void write(Object value) {

    }

    public void registerForNotify() {

    }
}
