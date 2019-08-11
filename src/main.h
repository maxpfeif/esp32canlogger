#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include "../lib/esp32_can/src/esp32_can.h"
#include "../lib/jeep_ehps/jeep_ehps.h"

void log_msg(CAN_FRAME*  printMsg);

#endif 