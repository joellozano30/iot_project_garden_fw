#pragma once

#define SHT31_ADDRESS 0x44
#define offsetTemperatura 0

void sht_init(void);
float get_calc_percentage_humidity(void);