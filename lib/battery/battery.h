#pragma once

#ifndef TEST
    #include "Arduino.h"
#else
    #include <ArduinoFake.h>
    using namespace fakeit;
#endif

#define BATTERY_PIN A2
#define MAX_VALUE 265
#define MIN_VALUE 215

void battery_init(void);
uint16_t calc_battery_level(void);
float get_calc_percentage_battery(void);