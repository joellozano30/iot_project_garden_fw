#pragma once

#define HUMIDITY_PIN A1
#define HUMEDAD_TIERRA 117  
#define HUMEDAD_AGUA 384

void humidity_init(void);
int calc_humidity(void);
float get_calc_percentage_humidity(void);