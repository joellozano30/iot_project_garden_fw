#pragma once

#ifndef TEST
    #include "Arduino.h"
#else
    #include <ArduinoFake.h>
    using namespace fakeit;
#endif

#define HUMIDITY_PIN A1
#define HUMEDAD_TIERRA 91  
#define HUMEDAD_AGUA 384

void humidity_init(void);
int calc_humidity(void);
float get_calc_percentage_humidity(void);