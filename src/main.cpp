#include <serial/Communicator.h>
#include "Arduino.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

typedef BLEClient* BLEClientPtr;

Communicator communicator(Serial);

SimpleMap<String, BLEClientPtr> *devices;

void setupCommands();

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");

    setupCommands();

    devices = new SimpleMap<String, BLEClientPtr>([](String &a, String &b) -> int {
        if (a == b) return 0;
        if (a > b) return 1;
        /*if (a < b) */ return -1;
    });
}

void loop() {
    communicator.update();
}

void setupCommands() {
    communicator.registerCommand("scan", [&](const String &params) {
        int interval;
        int window;
        int time;
        char serviceAddress[37]; // 36 plus 1 for null

        if (sscanf(params.c_str(), "%d %d %d %s", &interval, &window, &time, serviceAddress) < 0)
            return -1;

        auto bleServiceAddress = BLEUUID(serviceAddress);

        BLEScan *scan = BLEDevice::getScan();
        scan->setInterval(interval);
        scan->setWindow(window);
        scan->setActiveScan(true);
        BLEScanResults foundDevices = scan->start(time, false);

        for (int i = 0; i < foundDevices.getCount(); i++) {
            auto device = foundDevices.getDevice(i);

            if (!device.isAdvertisingService(bleServiceAddress))
                continue;

            communicator.getSerial().print(device.getAddress().toString().c_str());
            communicator.getSerial().print(" ");
            communicator.getSerial().println(device.getName().c_str());
        }

        return 0;
    });

    communicator.registerCommand("connect", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null

        if (sscanf(params.c_str(), "%s", deviceAddress) < 0)
            return -1;

        auto bleDeviceAddress = BLEAddress(deviceAddress);

        BLEClientPtr client = BLEDevice::createClient();
        if(client->connect(bleDeviceAddress)) {
            devices->put(deviceAddress, client);
            communicator.getSerial().println("connected");
        } else {
            communicator.getSerial().println("could not connect");
        }

        return 0;
    });

    communicator.registerCommand("disconnect", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null

        if (sscanf(params.c_str(), "%s", deviceAddress) < 0)
            return -1;

        BLEClientPtr client = devices->get(deviceAddress);
        if(client == nullptr) {
            communicator.getSerial().println("client not found");
            return 0;
        }

        client->disconnect();
        communicator.getSerial().println("disconnected");

        return 0;
    });

    communicator.registerCommand("read", [&](const String &params) {
        return 0;
    });

    communicator.registerCommand("write", [&](const String &params) {
        return 0;
    });

    communicator.registerCommand("register", [&](const String &params) {
        return 0;
    });
}