package ch.broox.ble;

public class BLEDevice {
    private BLESerialBridgeDriver driver;
    private String id;
    private boolean connected;

    public BLEDevice(BLESerialBridgeDriver driver, String id) {
        this.driver = driver;
        this.id = id;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public boolean isConnected() {
        return connected;
    }

    public void setConnected(boolean connected) {
        this.connected = connected;
    }
}
