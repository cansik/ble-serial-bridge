//
// Created by Bruggisser Florian on 11.08.20.
//

#ifndef TACTILEDEVICE_COMMUNICATOR_H
#define TACTILEDEVICE_COMMUNICATOR_H

#include <Stream.h>
#include <SimpleMap.h>
#include <HardwareSerial.h>
#include <cstdio>

class Communicator {
    static const char TOKEN_DELIMITER = ' ';
    static const char LINE_DELIMITER = '\n';

private:
    HardwareSerial &serial;
    SimpleMap<String, std::function<int(String)>>* commands;

public:
    explicit Communicator(HardwareSerial &serial);

    void update();

    void registerCommand(const String& command, const std::function<int(String)>& block);

    Stream &getSerial() const;
};


#endif //TACTILEDEVICE_COMMUNICATOR_H
