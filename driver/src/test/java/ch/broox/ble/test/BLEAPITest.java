package ch.broox.ble.test;

import ch.broox.ble.BLECharacteristic;
import ch.broox.ble.BLEDevice;
import ch.broox.ble.BLEDriver;
import ch.broox.ble.BLEService;
import org.junit.Test;

public class BLEAPITest {

    @Test
    public void testHighLevel() {
        try (BLEDriver driver = new BLEDriver()) {
            driver.open(TestUtils.ADDRESS, TestUtils.BAUD_RATE);

            BLEDevice device = new BLEDevice(driver, TestUtils.DEVICE_ID);
            device.connect();

            BLEService service = device.getService(TestUtils.SERVICE_ID);
            BLECharacteristic characteristic = service.getCharacteristic(TestUtils.NEOPIXEL_COLOR_ID);

            characteristic.writeUInt32(0x550010);

            TestUtils.sleep(500);

            device.disconnect();
        }
    }
}
