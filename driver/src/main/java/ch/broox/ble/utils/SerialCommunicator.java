package ch.broox.ble.utils;

import com.fazecast.jSerialComm.SerialPort;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Semaphore;
import java.util.regex.Pattern;

public class SerialCommunicator implements AutoCloseable {
    private volatile boolean open = false;
    private SerialPort port;
    private OutputStreamWriter out;
    private Thread communicationThread;

    private final List<SerialMessageListener> messageListeners = new ArrayList<>();
    private final String streamDelimiter;
    private final String notifyKeyword = "notify";

    // multi threading
    private volatile String recentContent = "";
    private final Semaphore mutex = new Semaphore(0);
    private long requestTimestamp = Long.MAX_VALUE;
    private long timeout = 1000 * 10;

    public SerialCommunicator() {
        this("]");
    }

    public SerialCommunicator(String streamDelimiter) {
        this.streamDelimiter = streamDelimiter;
    }

    public boolean open(String portAddress, int baudRate) {
        if(open) return false;

        port =  SerialPort.getCommPort(portAddress);
        port.setBaudRate(baudRate);

        if(port.openPort(100)) {
            port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0);

            OutputStream output = port.getOutputStream();
            out = new OutputStreamWriter(output);

            communicationThread = new Thread(this::receiverLoop);
            communicationThread.setDaemon(true);
            communicationThread.start();

            open = true;
        }

        return false;
    }

    public void close() {
        if(!open) return;

        open = false;

        try {
            communicationThread.join(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        port.closePort();
    }

    public void addMessageListener(SerialMessageListener listener) {
        messageListeners.add(listener);
    }

    public void removeMessageListener(SerialMessageListener listener) {
        messageListeners.remove(listener);
    }

    public void sendCommand(String command) {
        try {
            out.write(command + "\n");
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public synchronized String sendCommandAndAwait(String command) {
        sendCommand(command);

        // await
        try {
            requestTimestamp = System.currentTimeMillis();
            mutex.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return recentContent;
    }

    private void notifyMessageListener(String message) {
        for(SerialMessageListener listener : messageListeners) {
            listener.received(message);
        }
    }

    private void receiverLoop() {
        InputStream in = port.getInputStream();
        Scanner scan = new Scanner(in);
        scan.useDelimiter(Pattern.compile(streamDelimiter));

        while(open) {
            if(scan.hasNext()) {
                String logicalLine = scan.next();
                String content = logicalLine.substring(logicalLine.lastIndexOf("[") + 1).trim();

                if(content.startsWith(notifyKeyword)) {
                    String notification = content.substring(notifyKeyword.length() + 1);
                    notifyMessageListener(notification);
                    continue;
                }

                recentContent = content;
                mutex.release();
            }

            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // check for timeout
            if(mutex.hasQueuedThreads() && System.currentTimeMillis() - requestTimestamp > timeout) {
                recentContent = "error operation timed out";
                mutex.release();
            }
        }

        scan.close();
    }

    public long getTimeout() {
        return timeout;
    }

    public void setTimeout(long timeout) {
        this.timeout = timeout;
    }
}
