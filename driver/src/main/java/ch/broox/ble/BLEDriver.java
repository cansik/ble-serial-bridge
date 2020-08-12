package ch.broox.ble;

import ch.broox.ble.utils.SerialCommunicator;
import ch.broox.ble.utils.SerialMessageListener;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class BLEDriver implements AutoCloseable {
    private static String TOKEN_DELIMITER = " ";
    private static String LINE_DELIMITER = "\n";
    private static String OK_RESULT = "ok";
    private static String ERROR_RESULT = "error";

    protected static String FORMAT_STRING = "str";
    protected static String FORMAT_UINT8 = "i8";
    protected static String FORMAT_UINT16 = "i16";
    protected static String FORMAT_UINT32 = "i32";

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

    public List<BLEDevice> scan(int interval, int window, int time, String serviceId) {
        // increase time out for scanning process
        long timeout = communicator.getTimeout();
        communicator.setTimeout(timeout + time);
        String result = checkError(communicator.sendCommandAndAwait(String.format("scan %s %s %s %s", interval, window, time, serviceId)));
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
        String result = checkError(communicator.sendCommandAndAwait("list"));
        String[] lines = result.split(LINE_DELIMITER);

        return Arrays.stream(lines).map(line -> new BLEDevice(this, line.strip())).collect(Collectors.toList());
    }

    public String read(String deviceId, String serviceId, String characteristicsId, String format) {
        return checkError(communicator.sendCommandAndAwait(String.format("read %s %s %s %s", deviceId, serviceId, characteristicsId, format)));
    }

    public void write(String deviceId, String serviceId, String characteristicsId, String format, Object value) {
        checkError(communicator.sendCommandAndAwait(String.format("write %s %s %s %s %s", deviceId, serviceId, characteristicsId, value, format)));
    }

    public void registerForNotify(String deviceId, String serviceId, String characteristicsId) {
        checkError(communicator.sendCommandAndAwait(String.format("register %s %s %s", deviceId, serviceId, characteristicsId)));
    }

    public void addNotifyListener(SerialMessageListener listener) {
        communicator.addMessageListener(listener);
    }

    public void removeNotifyListener(SerialMessageListener listener) {
        communicator.addMessageListener(listener);
    }

    private String checkError(String content) {
        if(content.startsWith(ERROR_RESULT)) {
            throw new BLEException(content.substring(ERROR_RESULT.length() + 1));
        }
        return content;
    }
}
