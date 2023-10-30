#include "rtc.h"

alarmConfigStruct txTimeMarker;

uint8_t txFlag;
static uint8_t checkFlag;
uint8_t timeToCheck;
uint8_t minInc=0, secInc=0;

void rtc_init(){
    txFlag=false;
    checkFlag=false;
}
void rtc_handler(void)
{
    datetimeStruct dts;

    timeToCheck = rtc_check_if_time_to_send();
    
    rtc_get_time(&dts);
    if((dts.hour == txTimeMarker.hours) && (dts.minute == txTimeMarker.minutes) && (dts.second == txTimeMarker.seconds))
        txFlag = true;
}

uint8_t rtc_check_if_time_to_send(void)
{
    datetimeStruct dts;
    uint8_t compareVal = 0;

    // This gets the number of minutes to compare with
    // TODO: should have an enum for this values
    compareVal = dev_utils_get_tx_time_in_mins();

    rtc_get_time(&dts);
    if((dts.minute % compareVal == 0) && !checkFlag)
    {
        #ifndef TEST
        rtc_get_random_time_to_send(&minInc, &secInc);
        #endif
        rtc_calc_new_alarm_vals(&txTimeMarker, (alarmConfigStruct){0, minInc, secInc});
        #ifndef TEST
        rtc_print_alarm_struct(txTimeMarker);
        #endif
        checkFlag = true;
        return true;
    }
    else if((dts.minute % compareVal != 0) && checkFlag)
    {
        checkFlag = false;
        return false;
    }

    return false;
}   

void rtc_calc_new_alarm_vals(alarmConfigStruct *acs, alarmConfigStruct increment)
{
    datetimeStruct dts;
    rtc_get_time(&dts);

    dts.second += (uint8_t)increment.seconds;
    if(dts.second >=60)
    {
        dts.minute++;
        dts.second = dts.second - 60;
    }

    dts.minute += (uint8_t)increment.minutes;
    if(dts.minute >= 60)
    {
        dts.hour++;
        dts.minute = dts.minute - 60;
    }

    dts.hour += (uint8_t)increment.hours;
    if(dts.hour >=24)
    {
        dts.hour = dts.hour - 24;
    }

    acs->hours = (int)dts.hour;
    acs->minutes = (int)dts.minute;
    acs->seconds = (int)dts.second;
}


void rtc_get_random_time_to_send(uint8_t *minuteIncrement, uint8_t *secondIncrement)
{

    long randomNumber = random(60);
    int value = (int)randomNumber % RAND_SEC_MAX;

    uint8_t txWindowVal = dev_utils_get_tx_time_in_mins();
    txWindowVal = (txWindowVal / 5);

    #ifndef DONT_USE_RANDOM_FOR_TX
    *secondIncrement = (uint8_t)value;
    *minuteIncrement = ((int)randomNumber % txWindowVal) + 1;
    #else
    *secondIncrement = 10;
    *minuteIncrement = 0;
    #endif

    Serial.print("[*] Random Value for minutes: ");
    Serial.println((uint16_t)*minuteIncrement);
    Serial.print("[*] Random Value for seconds: ");
    Serial.println((uint16_t)*secondIncrement);
}

void rtc_print_alarm_struct(alarmConfigStruct dts)
{    
    Serial.print("[*] SE ENVIARA A LAS -> ");
    Serial.print(dts.hours);
    Serial.print(":");
    Serial.print(dts.minutes);
    Serial.print(":");
    Serial.println(dts.seconds);
}

void rtc_get_time(datetimeStruct *dt){
    #ifndef TEST
    dt->hour = hour();
    dt->minute = minute();
    dt->second = second();
    #else 
    dt->hour = 3;
    dt->minute = 30;
    dt->second = 59;    
    #endif
}

void rtc_show_time(void){

    datetimeStruct dts;
    rtc_get_time(&dts);

    Serial.print("[*] Current Time -> ");
    Serial.print(dts.hour);
    Serial.print(":");
    Serial.print(dts.minute);
    Serial.print(":");
    Serial.println(dts.second);

}

uint8_t dev_utils_get_tx_time_in_mins(void)
{
    #ifdef USE_TRANSMISSION_PERIOD_DEFINE
    return TRANSMISSION_CODE;
    #else
    uint8_t timeVal;
    switch(tx_window_code)
    {
        case 0:
            timeVal = 1;
            break;
        case 1:
            timeVal = 2;
            break;
        case 2:
            timeVal = 5;
            break;
        case 3:
            timeVal = 10;
            break;
        case 4:
            timeVal = 15;
            break;
        case 5:
            timeVal = 20;
            break;
        default:
            timeVal = 15;
            break;
    }

    return timeVal;
    #endif
}

void rtc_set_tx_flag(uint8_t val)
{
    txFlag = val;
}

uint8_t rtc_get_tx_flag(void)
{
    return txFlag;
}