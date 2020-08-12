package ch.broox.ble.test;

public class TestUtils {
    public static String ADDRESS = "/dev/tty.usbserial-14410";
    public static int BAUD_RATE = 115200;

    public static String DEVICE_ID = "30:ae:a4:14:e7:62";
    public static String SERVICE_ID = "846123f6-ccf1-11ea-87d0-0242ac130003";
    public static String NEOPIXEL_COLOR_ID = "fc3affa6-5020-47ce-93db-2e9dc45c9b55";
    public static String IMU_ID = "98f09e34-73ab-4f2a-a5eb-a95e7e7ab733";

    public static void sleep(long ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
