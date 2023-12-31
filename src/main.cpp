#include <Arduino.h>
//#include <SoftwareSerial.h>
#include "sigfox.h"
#include "humidity.h"
#include "temperature.h"
#include "rtc.h"

#define RX_BUFFER_SIZE 100
#define FIVE_SECONDS 5000
#define ONE_MINUTE 60000UL
#define ONE_SECOND 1000UL

datetimeStruct dts;

String msgBuffer = "";
char rxBuffer[RX_BUFFER_SIZE] = {'\0'};
unsigned long start_time;
unsigned long start_time2;
int num_data = 0;
float humidity_floor = 0;
float temperature = 0;
float humidity_to_send = 0;
float temperature_to_send = 0;
float battery_to_send = 0;

void mainSendSigfoxMessage(sigfox_msg_type msgType);

void setup() {

  Serial.begin(9600);
  humidity_init();
  Serial.println("[*] Hello manito!"); 
  Serial.println("[*] Starting Sigfox Configuration"); 

  rtc_init();
  sigfoxInit();
  delay(2000);
  sigfoxReadInfo();
  delay(1000);

  humidity_to_send = 0;
  temperature_to_send = 0;
  battery_to_send = 0;

  mainSendSigfoxMessage(BIDIR_MSG);

  start_time = millis();
  start_time2 = millis();

  randomSeed(analogRead(A0));
  Serial.println("[*] Finish Setup!"); 
  sigfox_enter_sleep_mode();
}

void loop() {

  if((millis()-start_time) > ONE_SECOND){
    rtc_handler();
    rtc_show_time();
    start_time = millis();
  }

  if((millis()-start_time2)>ONE_MINUTE){
    Serial.println("[*] Calculating Parameters: ");
    humidity_floor+=get_calc_percentage_humidity();
    //temperature+=get_temperature();
    //
    Serial.print("[*] Humidity: ");
    Serial.println(get_calc_percentage_humidity());
    start_time2 = millis();
    num_data++;
  }

  if(rtc_get_tx_flag())
  {
      //serial1_printConstStr("[*] Inside Sigfox Window\r\n");  
    Serial.println("[*] Inside Sigfox Window\r\n");    
    rtc_set_tx_flag(false);
    sigfoxInit();
    humidity_to_send = humidity_floor/(float)num_data; //mean
    temperature_to_send = temperature/(float)num_data; //mean

    Serial.print("[*] Humidity average: ");
    Serial.println(humidity_to_send);

    if(sx_get_dwnrec_flag())
      mainSendSigfoxMessage(UNIDIR_MSG);
    else 
      mainSendSigfoxMessage(BIDIR_MSG);

    num_data = 0;
    humidity_floor = 0;
    temperature = 0;
    sigfox_enter_sleep_mode();
  }

}

void mainSendSigfoxMessage(sigfox_msg_type msgType){

  sigfoxPackMsg(humidity_to_send, temperature_to_send, battery_to_send, &msgBuffer);
  Serial.print("[!] Message to Send: "); 
  Serial.println(msgBuffer); 
  
  if(msgType == UNIDIR_MSG){
    sigfoxSendMsg(msgBuffer);
  }
  else if(msgType == BIDIR_MSG)
  {
    sigfoxSendBidirMsg(msgBuffer, rxBuffer);
    sigfoxParseResponse(rxBuffer);
  }

}