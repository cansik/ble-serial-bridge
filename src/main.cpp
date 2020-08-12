#include <serial/Communicator.h>
#include "Arduino.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define DEBUG_MODE

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
            communicator.getSerial().print("connected");
            return 0;
        }

        communicator.getSerial().print("could not connect");
        return 1;
    });

    communicator.registerCommand("disconnect", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null

        if (sscanf(params.c_str(), "%s", deviceAddress) < 0)
            return -1;

        auto client = devices->get(deviceAddress);
        if (client == nullptr) {
            communicator.getSerial().print("client not found");
            return 1;
        }

        client->disconnect();
        devices->remove(deviceAddress);
        free(client);

        communicator.getSerial().print("disconnected");

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
        char format[4] = "str"; // 3 plus 1 for null

        if (sscanf(params.c_str(), "%s %s %s %s", deviceAddress, serviceAddress, characteristicAddress, format) < -1)
            return -1;

        auto client = devices->get(deviceAddress);
        auto serviceUUID = BLEUUID(serviceAddress);
        auto charUUID = BLEUUID(characteristicAddress);
        if (client == nullptr) {
            communicator.getSerial().print("error client not found");
            return 1;
        }

        auto service = client->getService(serviceUUID);
        if (service == nullptr) {
            communicator.getSerial().print("error service not found");
            return 1;
        }

        auto characteristic = service->getCharacteristic(charUUID);
        if (characteristic == nullptr) {
            communicator.getSerial().print("error characteristic not found");
            return 1;
        }

        if (!characteristic->canRead()) {
            communicator.getSerial().print("error characteristic not readable");
            return 1;
        }

        // read value
        if(startsWith("str", format)) {
            std::string value = characteristic->readValue();
            communicator.getSerial().println(value.c_str());
            return 0;
        }

        if(startsWith("i8", format)) {
            auto value = characteristic->readUInt8();
            communicator.getSerial().println(value);
            return 0;
        }

        if(startsWith("i16", format)) {
            auto value = characteristic->readUInt16();
            communicator.getSerial().println(value);
            return 0;
        }

        if(startsWith("i32", format)) {
            auto value = characteristic->readUInt32();
            communicator.getSerial().println(value);
            return 0;
        }

        return 0;
    });

    communicator.registerCommand("write", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null
        char serviceAddress[37]; // 36 plus 1 for null
        char characteristicAddress[37]; // 36 plus 1 for null
        char value[65]; // 64 plus 1 for null (should be 512 + 1)
        char format[4] = "str"; // 3 plus 1 for null

        if (sscanf(params.c_str(), "%s %s %s %s %s",
                   deviceAddress, serviceAddress, characteristicAddress, value, format) < 1)
            return -1;

        auto client = devices->get(deviceAddress);
        auto serviceUUID = BLEUUID(serviceAddress);
        auto charUUID = BLEUUID(characteristicAddress);
        if (client == nullptr) {
            communicator.getSerial().print("error client not found");
            return 1;
        }

        auto service = client->getService(serviceUUID);
        if (service == nullptr) {
            communicator.getSerial().print("error service not found");
            return 1;
        }

        auto characteristic = service->getCharacteristic(charUUID);
        if (characteristic == nullptr) {
            communicator.getSerial().print("error characteristic not found");
            return 1;
        }

        if (!characteristic->canWrite()) {
            communicator.getSerial().print("error characteristic not writable");
            return 1;
        }

        // write
        if(startsWith("str", format)) {
            characteristic->writeValue(value, false);
            communicator.getSerial().print("value written");
            return 0;
        }

        if(startsWith("i8", format)) {
            uint8_t number = String(value).toInt();
            characteristic->writeValue(&number, 1, false);

            communicator.getSerial().print("value written");
            return 0;
        }

        if(startsWith("i16", format)) {
            uint16_t number = String(value).toInt();
            byte *b = (byte *)&number;

            characteristic->writeValue(b, 2, false);
            communicator.getSerial().print("value written");
            return 0;
        }

        if(startsWith("i32", format)) {
            uint32_t number = String(value).toInt();
            byte *b = (byte *)&number;

            characteristic->writeValue(b, 4, false);
            communicator.getSerial().print("value written");
            return 0;
        }

        communicator.getSerial().print("error could not write value");
        return 1;
    });

    communicator.registerCommand("register", [&](const String &params) {
        char deviceAddress[18]; // 17 plus 1 for null
        char serviceAddress[37]; // 36 plus 1 for null
        char characteristicAddress[37]; // 36 plus 1 for null
        char format[4] = "str"; // 3 plus 1 for null

        if (sscanf(params.c_str(), "%s %s %s %s", deviceAddress, serviceAddress, characteristicAddress, format) < -1)
            return -1;

        auto client = devices->get(deviceAddress);
        auto serviceUUID = BLEUUID(serviceAddress);
        auto charUUID = BLEUUID(characteristicAddress);
        if (client == nullptr) {
            communicator.getSerial().print("error client not found");
            return 1;
        }

        auto service = client->getService(serviceUUID);
        if (service == nullptr) {
            communicator.getSerial().print("error service not found");
            return 1;
        }

        auto characteristic = service->getCharacteristic(charUUID);
        if (characteristic == nullptr) {
            communicator.getSerial().print("error characteristic not found");
            return 1;
        }

        if (!characteristic->canNotify()) {
            communicator.getSerial().print("error characteristic not notifiable");
            return 1;
        }

        characteristic->registerForNotify([](BLERemoteCharacteristic* ch,
                                             uint8_t* data,
                                             size_t length,
                                             bool isNotify) {
            communicator.getSerial().print("notify: ");
            unsigned int value = *reinterpret_cast<unsigned int*>(data);
            communicator.getSerial().println(value);
        });

        communicator.getSerial().print("notify registered");

        return 0;
    });
}