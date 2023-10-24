#ifndef SIGFOX_H
#define SIGFOX_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <TimeLib.h>

#define SIGFOX_RX D7
#define SIGFOX_TX D8

#define SIGFOX_ENABLE 7
#define SEVENTY_FIVE_SECONDS 75000UL
#define FIFTY_SECONDS 50000UL

typedef enum
{
    UNIDIR_MSG = 1,
    BIDIR_MSG
}sigfox_msg_type;

void sigfoxInit(void);
void sigfoxChangeToRegion4(void);
void sigfoxReadInfo(void);
void sigfoxSendATCommand(char* comandoAT);
void sigfoxPackMsg(float humidity, float temperature, float battery, String *dataMsg);
void sigfoxSendMsg(String buf_tx);
void sigfoxSendBidirMsg(String buf_tx, char *buf_rx);
void sigfoxReadResponse(char *buf_rx);
void sigfoxParseResponse(char* buf_rx);
void sigfox_enter_sleep_mode(void);
void sx_set_dwnrec_flag(uint8_t val);
uint8_t sx_get_dwnrec_flag(void);

#endif