package ch.broox.ble;

public class BLEDevice {
    private BLEDriver driver;
    private String id;
    private String name;
    private boolean connected;

    public BLEDevice(BLEDriver driver, String id, String name) {
        this.driver = driver;
        this.id = id;
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public boolean isConnected() {
        return connected;
    }

    public void setConnected(boolean connected) {
        this.connected = connected;
    }
}
