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

    setupCommands();
}

void Communicator::update() {
    // read serial only if available
    if(!serial.available())
        return;

    // read line
    auto line = serial.readStringUntil(LINE_DELIMITER);
    line.trim();

    // trim command
    auto command = line.substring(0, line.indexOf(TOKEN_DELIMITER));
    auto params = line.substring(line.indexOf(TOKEN_DELIMITER) + 1);

    serial.print("command: ");
    serial.println(command);

    serial.print("params: ");
    serial.println(params);

    auto block = commands->get(command);


    if(block == nullptr) {
        serial.println("Command is not registered!");
        return;
    }

    // run command
    auto output = block(params);

    serial.print("Output: ");
    serial.println(output);

    // check output
    if(output == -1) {
        serial.println("Error in parsing parameters!");
    }
}

void Communicator::setupCommands() {
    // add commands
    commands->put("test", [&] (const String& params) {
        char message[20];

        if(sscanf(params.c_str(), "%s", message) < 0)
            return -1;

        serial.print("Test MSG: ");
        serial.println(message);

        return 0;
    });
}

void Communicator::registerCommand(const String& command, const std::function<int(String)>& block) {
    commands->put(command, block);
}

Stream &Communicator::getSerial() const {
    return serial;
}
