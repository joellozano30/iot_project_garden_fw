#include "temperature.h"

#ifndef TEST
SHT31 sht;
#endif

void sht_init()
{
    Wire.begin();
    #ifndef TEST
    sht.begin(SHT31_ADDRESS);
    #else
        //sht_begin();
    #endif
    
    Wire.setClock(100000);
}

float get_temperature()
{
    #ifndef TEST
    sht.read();
    #else
        //sht_read();
    #endif

    // Aplicar correcciones de calibración
    #ifndef TEST
    float temperaturaCalibrada = sht.getTemperature() + offsetTemperatura;
    #else   
    float temperaturaCalibrada = /*sht_getTemperature() + */offsetTemperatura;
    #endif

    return temperaturaCalibrada;
}
