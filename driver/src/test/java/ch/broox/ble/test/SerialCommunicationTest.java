package ch.broox.ble.test;

import ch.broox.ble.utils.SerialCommunicator;
import org.junit.Test;

public class SerialCommunicationTest {
    String address = "/dev/tty.usbserial-14410";
    int baudRate = 115200;

    @Test
    public void testLowLevelReading() throws InterruptedException {
        SerialCommunicator communicator = new SerialCommunicator();

        communicator.open(address, baudRate);

        System.out.println("send ls:");
        String result = communicator.sendCommandAndAwait("list");
        System.out.println("Result: '" + result + "'");

        communicator.close();
    }
}
