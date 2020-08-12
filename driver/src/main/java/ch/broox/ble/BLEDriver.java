package ch.broox.ble;

import ch.broox.ble.utils.SerialCommunicator;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class BLEDriver implements AutoCloseable {
    private static String TOKEN_DELIMITER = " ";
    private static String LINE_DELIMITER = "\n";
    private static String OK_RESULT = "ok";
    private static String ERROR_RESULT = "error";

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
        // increase time out for scanning process
        long timeout = communicator.getTimeout();
        communicator.setTimeout(timeout + time);
        String result = checkError(communicator.sendCommandAndAwait(String.format("scan %s %s %s %s", interval, window, time, serviceAddress)));
        communicator.setTimeout(timeout);

        String[] lines = result.split(LINE_DELIMITER);

        return Arrays.stream(lines).map(line -> {
                    String[] tokens = line.split(TOKEN_DELIMITER);
                    return new BLEDevice(this, tokens[0], (tokens.length > 1) ? tokens[1] : "");
                }).collect(Collectors.toList());
    }

    public void connect(String deviceId) {
        checkError(communicator.sendCommandAndAwait(String.format("connect %s", deviceId)));
    }

    public void disconnect(String deviceId) {
        checkError(communicator.sendCommandAndAwait(String.format("disconnect %s", deviceId)));
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

    private String checkError(String content) {
        if(content.startsWith(ERROR_RESULT)) {
            throw new BLEException(content.substring(ERROR_RESULT.length() + 1));
        }
        return content;
    }
}
