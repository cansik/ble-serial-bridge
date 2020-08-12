package ch.broox.ble;

import java.util.List;

public class BLESerialBridgeDriver {
    private volatile boolean open = false;

    public boolean open(String port, int baudRate) {
        if (open) return false;

        return true;
    }

    public boolean close() {
        if (!open) return false;

        return true;
    }


    public List<BLEDevice> scan(int interval, int window, int time, String serviceAddress) {
        return null;
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
