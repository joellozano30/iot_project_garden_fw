#include "humidity.h"

void humidity_init(){
    pinMode(HUMIDITY_PIN,INPUT);
}
int calc_humidity(){
    int humidity=analogRead(HUMIDITY_PIN);
    return humidity;
}

float get_calc_percentage_humidity(){
    int humidity_measured=calc_humidity();
    float percentage_humidity=map((long)humidity_measured, HUMEDAD_TIERRA, HUMEDAD_AGUA, 0, 100);
    if(percentage_humidity>100)
        percentage_humidity=100;
    if(percentage_humidity < 0)
        percentage_humidity=0;
    return percentage_humidity;
}