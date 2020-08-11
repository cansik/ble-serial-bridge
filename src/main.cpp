#include <serial/Communicator.h>
#include "Arduino.h"

Communicator communicator(Serial);

void setup() {
    Serial.begin(115200);
}

void loop() {
    communicator.update();
}