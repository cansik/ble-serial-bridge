package ch.broox.ble;

public class BLEService {
    private BLEDevice device;
    private String id;

    public BLEService(BLEDevice device, String id) {
        this.device = device;
        this.id = id;
    }

    public BLECharacteristic getCharacteristic(String id) {
        return new BLECharacteristic(this, id);
    }

    public BLEDevice getDevice() {
        return device;
    }

    public String getId() {
        return id;
    }
}
