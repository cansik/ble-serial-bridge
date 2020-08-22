//
// Created by Bruggisser Florian on 11.08.20.
//

#include "Communicator.h"

Communicator::Communicator(HardwareSerial &serial) : serial(serial) {
    // setup commands
    commands = new SimpleMap<String, std::function<int(String)>>([](String& a, String& b) -> int {
        if (a == b) return 0;
        if (a > b) return 1;
        /*if (a < b) */ return -1;
    });
}

void Communicator::update() {
    // read serial only if available
    if(!serial.available())
        return;

    serial.print("[");

    // read line
    auto line = serial.readStringUntil(LINE_DELIMITER);
    line.trim();

    // trim command
    auto command = line.substring(0, line.indexOf(TOKEN_DELIMITER));
    auto params = line.substring(line.indexOf(TOKEN_DELIMITER) + 1);

    auto block = commands->get(command);


    if(block == nullptr) {
        serial.print("error Command is not registered!");
        serial.println("]");
        return;
    }

    // run command
    auto output = block(params);

    // check output
    if(output == -1) {
        serial.print("error Parameters could not be parsed!");
    }

    if(output == -2) {
        serial.print("error ");
        serial.print(errorMessage.c_str());
    }

    if(output == 0) {
        serial.print("ok");
    }

    serial.println("]");
}

void Communicator::registerCommand(const String& command, const std::function<int(String)>& block) {
    commands->put(command, block);
}

Stream &Communicator::getSerial() const {
    return serial;
}

const String &Communicator::getErrorMessage() const {
    return errorMessage;
}

void Communicator::setErrorMessage(const String &errorMessage) {
    Communicator::errorMessage = errorMessage;
}
