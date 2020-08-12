package ch.broox.ble.test;

import ch.broox.ble.BLEDevice;
import ch.broox.ble.BLEDriver;
import org.junit.Test;

import java.util.List;

public class BLETests {

    @Test
    public void scanTest() {
        try (BLEDriver driver = new BLEDriver()) {
            driver.open(TestUtils.ADDRESS, TestUtils.BAUD_RATE);
            List<BLEDevice> devices = driver.scan(100, 99, 2, TestUtils.SERVICE_ID);

            for(BLEDevice device : devices) {
                System.out.println(device.getId() + " (" + device.getName() + ")");
            }
        }
    }

    @Test
    public void connectAndDisconnect() {
        try (BLEDriver driver = new BLEDriver()) {
            driver.open(TestUtils.ADDRESS, TestUtils.BAUD_RATE);

            driver.connect(TestUtils.DEVICE_ID);
            TestUtils.sleep(2000);
            driver.disconnect(TestUtils.DEVICE_ID);
        }
    }

    @Test
    public void readCharacteristics() {
        try (BLEDriver driver = new BLEDriver()) {
            driver.open(TestUtils.ADDRESS, TestUtils.BAUD_RATE);

            driver.connect(TestUtils.DEVICE_ID);
            String value = driver.read(TestUtils.DEVICE_ID, TestUtils.SERVICE_ID, TestUtils.NEOPIXEL_COLOR_ID, "i32");
            System.out.println("Value read: " + value);
            driver.disconnect(TestUtils.DEVICE_ID);
        }
    }

    @Test
    public void writeCharacteristics() {
        try (BLEDriver driver = new BLEDriver()) {
            driver.open(TestUtils.ADDRESS, TestUtils.BAUD_RATE);

            driver.connect(TestUtils.DEVICE_ID);
            driver.write(TestUtils.DEVICE_ID, TestUtils.SERVICE_ID, TestUtils.NEOPIXEL_COLOR_ID, "i32", 0x0000FF);
            driver.disconnect(TestUtils.DEVICE_ID);
        }
    }
}
