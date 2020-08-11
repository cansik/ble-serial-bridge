#include <serial/Communicator.h>
#include "Arduino.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

Communicator communicator(Serial);

void setupCommands();

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");

    setupCommands();
}

void loop() {
    communicator.update();
}

void setupCommands() {
    communicator.registerCommand("scan", [&] (const String& params) {
        int interval;
        int window;
        int time;
        char uuidString[37]; // 36 plus 1 for null

        if(sscanf(params.c_str(), "%d %d %d %s", &interval, &window, &time, uuidString) < 0)
            return -1;

        auto uuid = BLEUUID(uuidString);

        Serial.println(uuid.toString().c_str());

        BLEScan* scan = BLEDevice::getScan();
        scan->setInterval(interval);
        scan->setWindow(window);
        scan->setActiveScan(true);
        BLEScanResults foundDevices = scan->start(time, false);

        for(int i = 0; i < foundDevices.getCount(); i++) {
            auto device = foundDevices.getDevice(i);

            if(!device.isAdvertisingService(uuid))
                continue;

            communicator.getSerial().print(device.getAddress().toString().c_str());
            communicator.getSerial().print(" ");
            communicator.getSerial().println(device.getName().c_str());
        }

        return 0;
    });
}