//
// Created by Bruggisser Florian on 11.08.20.
//

#ifndef TACTILEDEVICE_COMMUNICATOR_H
#define TACTILEDEVICE_COMMUNICATOR_H

#include <Stream.h>
#include <SimpleMap.h>

class Communicator {
    static const char TOKEN_DELIMITER = ' ';
    static const char LINE_DELIMITER = '\n';

private:
    Stream &serial;
    SimpleMap<String, std::function<int(String)>>* commands;

    void setupCommands();

public:
    explicit Communicator(Stream &serial);

    void update();
};


#endif //TACTILEDEVICE_COMMUNICATOR_H
