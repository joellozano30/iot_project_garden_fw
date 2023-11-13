#include "battery.h"

void battery_init(){
    pinMode(BATTERY_PIN,INPUT);
}
uint16_t calc_battery_level(){
    uint16_t battery_level=analogRead(BATTERY_PIN);
    return battery_level;
}

float get_calc_percentage_battery(){
    uint16_t voltage_bat_measured=calc_battery_level();
    float percentage_battery=map((long)voltage_bat_measured, MAX_VALUE, MIN_VALUE, 0, 100);
    if(percentage_battery>100)
        percentage_battery=100;
    if(percentage_battery < 0)
        percentage_battery=0;
    return percentage_battery;
}