#include "temperature.h"
#include "SHT31.h"
#include "Wire.h"

SHT31 sht;

void sht_init()
{
    Wire.begin();
    sht.begin(SHT31_ADDRESS);
    Wire.setClock(100000);
}

float get_calc_percentage_humidity()
{

    sht.read();

    // Aplicar correcciones de calibración
    float temperaturaCalibrada = sht.getTemperature() + offsetTemperatura;

    delay(100);
    return temperaturaCalibrada;
}
