#include <serial/Communicator.h>
#include "Arduino.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

typedef BLEClient *BLEClientPtr;

void setupCommands();
bool startsWith(const char *pre, const char *str);

Communicator communicator(Serial);
SimpleMap<String, BLEClientPtr> *devices;

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

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
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

        auto *scan = BLEDevice::getScan();
        scan->setInterval(interval);
        scan->setWindow(window);
        scan->setActiveScan(true);
        auto foundDevices = scan->start(time, false);

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

        auto client = BLEDevice::createClient();
        if (client->connect(bleDeviceAddress)) {
            devices->put(deviceAddress, client);
            communicator.getSerial().println("connected");
            return 0;
        }

        communicator.getSerial().println("could not connect");
        return 1;
    });

    communicator.registerCommand("disconnect", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null

        if (sscanf(params.c_str(), "%s", deviceAddress) < 0)
            return -1;

        auto client = devices->get(deviceAddress);
        if (client == nullptr) {
            communicator.getSerial().println("client not found");
            return 1;
        }

        client->disconnect();
        devices->remove(deviceAddress);
        free(client);

        communicator.getSerial().println("disconnected");

        return 0;
    });

    communicator.registerCommand("list", [&](const String &params) {
        for (int i = 0; i < devices->size(); i++) {
            BLEClientPtr client = devices->getData(i);

            // cleanup not connected clients
            if (!client->isConnected()) {
                devices->remove(i);
                free(client);
                continue;
            }

            communicator.getSerial().println(devices->getKey(i));
        }
        return 0;
    });

    communicator.registerCommand("read", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null
        char serviceAddress[37]; // 36 plus 1 for null
        char characteristicAddress[37]; // 36 plus 1 for null
        char format[4] = "val"; // 3 plus 1 for null

        if (sscanf(params.c_str(), "%s %s %s %s", deviceAddress, serviceAddress, characteristicAddress, format) < -1)
            return -1;

        auto client = devices->get(deviceAddress);
        auto serviceUUID = BLEUUID(serviceAddress);
        auto charUUID = BLEUUID(characteristicAddress);
        if (client == nullptr) {
            communicator.getSerial().println("client not found");
            return 1;
        }

        auto service = client->getService(serviceUUID);
        if (service == nullptr) {
            communicator.getSerial().println("service not found");
            return 1;
        }

        auto characteristic = service->getCharacteristic(charUUID);
        if (characteristic == nullptr) {
            communicator.getSerial().println("characteristic not found");
            return 1;
        }

        if (!characteristic->canRead()) {
            communicator.getSerial().println("characteristic not readable");
            return 1;
        }

        // read value
        if(startsWith("val", format)) {
            std::string value = characteristic->readValue();
            communicator.getSerial().println(value.c_str());
            return 0;
        }

        if(startsWith("ui8", format)) {
            auto value = characteristic->readUInt8();
            communicator.getSerial().println(value, HEX);
            return 0;
        }

        if(startsWith("i16", format)) {
            auto value = characteristic->readUInt16();
            communicator.getSerial().println(value, HEX);
            return 0;
        }

        if(startsWith("i32", format)) {
            auto value = characteristic->readUInt32();
            communicator.getSerial().println(value, HEX);
            return 0;
        }

        return 0;
    });

    communicator.registerCommand("write", [&](const String &params) {
        return 0;
    });

    communicator.registerCommand("register", [&](const String &params) {
        return 0;
    });
}