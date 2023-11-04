#pragma once

#ifndef TEST
    #include "Arduino.h"
    #include "Wire.h"
    #include "SHT31.h"
    
#else
    #include "ArduinoFake.h"
    #include "Wire.h"
    using namespace fakeit;
#endif

#define SHT31_ADDRESS 0x44
#define offsetTemperatura 0

void sht_init(void);
float get_temperature(void);