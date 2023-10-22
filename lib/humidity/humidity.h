#pragma once

#define HUMIDITY_PIN 22
#define HUMEDAD_AIRE 550 
#define HUMEDAD_AGUA 250

void humidity_init(void);
int calc_humidity(void);
float get_calc_percentage_humidity(void);