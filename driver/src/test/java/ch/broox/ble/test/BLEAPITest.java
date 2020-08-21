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
            BLECharacteristic characteristic = service.getCharacteristic(TestUtils.IR_LED_ID);

            // turn on ir led
            System.out.println("turn on");
            characteristic.writeUInt32(1);
            TestUtils.sleep(2000);
            System.out.println("State: " + characteristic.readUInt32());
            TestUtils.sleep(2000);
            System.out.println("turn off");
            characteristic.writeUInt32(0);
            TestUtils.sleep(2000);
            System.out.println("State: " + characteristic.readUInt32());
            device.disconnect();
        }
    }
}
