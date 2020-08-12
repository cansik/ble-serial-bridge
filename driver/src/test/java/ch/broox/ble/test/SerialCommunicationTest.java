package ch.broox.ble.test;

import ch.broox.ble.utils.SerialCommunicator;
import org.junit.Test;

public class SerialCommunicationTest {

    @Test
    public void testLowLevelReading() throws InterruptedException {
        SerialCommunicator communicator = new SerialCommunicator();

        communicator.open(TestSettings.ADDRESS, TestSettings.BAUD_RATE);

        System.out.println("send connect:");
        String result = communicator.sendCommandAndAwait("connect " + TestSettings.DEVICE_ID);
        System.out.println("Result: '" + result + "'");

        communicator.close();
    }
}
