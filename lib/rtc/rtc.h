#ifndef RTC_H
#define RTC_H

#ifndef TEST
#include <Arduino.h>
//#include "stdlib.h"
#include <Time.h>
#include <TimeLib.h>
#else
#include <ArduinoFake.h>
#include <Time.h>
#endif

#define USE_TRANSMISSION_PERIOD_DEFINE
#define TRANSMISSION_CODE 30

#define RAND_SEC_MAX    60

typedef struct{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}datetimeStruct;

typedef struct{
    int hours;
    int minutes;
    int seconds;
}alarmConfigStruct;

void rtc_init();
void rtc_handler(void);
uint8_t rtc_check_if_time_to_send(void);
void rtc_calc_new_alarm_vals(alarmConfigStruct *acs, alarmConfigStruct increment);
void rtc_get_random_time_to_send(uint8_t *minuteIncrement, uint8_t *secondIncrement);
void rtc_print_alarm_struct(alarmConfigStruct dts);
void rtc_get_time(datetimeStruct *dt);
void rtc_show_time(void);
uint8_t dev_utils_get_tx_time_in_mins(void);
void rtc_set_tx_flag(uint8_t val);
uint8_t rtc_get_tx_flag(void);


#endif