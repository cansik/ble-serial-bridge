package ch.broox.ble;

import ch.broox.ble.utils.SerialMessageListener;

import static ch.broox.ble.BLEDriver.*;

public class BLECharacteristic {
    private BLEService service;
    private String id;

    public BLECharacteristic(BLEService service, String id) {
        this.service = service;
        this.id = id;
    }

    public String read(String format) {
        return service.getDevice().getDriver().read(service.getDevice().getId(), service.getId(), id, format);
    }

    public String readString() {
        return read(FORMAT_STRING);
    }

    public int readUInt8() {
        return Integer.parseInt(read(FORMAT_UINT8));
    }

    public int readUInt16() {
        return Integer.parseInt(read(FORMAT_UINT16));
    }

    public int readUInt32() {
        return Integer.parseInt(read(FORMAT_UINT32));
    }

    public void write(String value, String format) {
        service.getDevice().getDriver().write(service.getDevice().getId(), service.getId(), id, format, value);
    }

    public void writeString(String value) {
        write(value, FORMAT_STRING);
    }

    public void writeUInt8(int value) {
        write(String.valueOf(value), FORMAT_STRING);
    }

    public void writeUInt16(int value) {
        write(String.valueOf(value), FORMAT_UINT16);
    }

    public void writeUInt32(int value) {
        write(String.valueOf(value), FORMAT_UINT32);
    }

    public void registerForNotification(SerialMessageListener listener) {
        service.getDevice().getDriver().addNotifyListener(listener);
        service.getDevice().getDriver().registerForNotify(service.getDevice().getId(), service.getId(), id);
    }

    public BLEService getService() {
        return service;
    }

    public String getId() {
        return id;
    }
}
