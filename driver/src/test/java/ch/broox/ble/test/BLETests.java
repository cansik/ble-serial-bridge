package ch.broox.ble.test;

import ch.broox.ble.BLEDevice;
import ch.broox.ble.BLEDriver;
import org.junit.Test;

import java.util.List;

public class BLETests {

    @Test
    public void scanTest() {
        try (BLEDriver driver = new BLEDriver()) {
            driver.open(TestSettings.ADDRESS, TestSettings.BAUD_RATE);
            List<BLEDevice> devices = driver.scan(100, 99, 2, TestSettings.SERVICE_ID);

            for(BLEDevice device : devices) {
                System.out.println(device.getId() + " (" + device.getName() + ")");
            }
        }
    }
}
