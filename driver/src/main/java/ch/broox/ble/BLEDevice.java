package ch.broox.ble;

import java.util.Objects;

public class BLEDevice {
    private BLEDriver driver;
    private String id;
    private String name;
    private boolean connected;

    public BLEDevice(BLEDriver driver, String id) {
        this(driver, id, "");
    }

    public BLEDevice(BLEDriver driver, String id, String name) {
        this.driver = driver;
        this.id = id;
        this.name = name;
    }

    public void connect() {
        driver.connect(this.id);
    }

    public void disconnect() {
        driver.disconnect(this.id);
    }

    public BLEService getService(String serviceId) {
        return new BLEService(this, serviceId);
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

    public BLEDriver getDriver() {
        return driver;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        BLEDevice device = (BLEDevice) o;
        return id.equals(device.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }
}
