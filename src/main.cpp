#ifndef TEST
  #include <Arduino.h>
#else
  #include <ArduinoFake.h>
#endif 

#include "sigfox.h"
#include "temperature.h"
#include "humidity.h"
#include "rtc.h"

#define RX_BUFFER_SIZE 100
#define FIVE_SECONDS 5000
#define ONE_MINUTE 60000UL
#define ONE_SECOND 1000UL

datetimeStruct dts;

String msgBuffer = "";
char rxBuffer[RX_BUFFER_SIZE] = {'\0'};
uint32_t start_time;
uint32_t start_time2;
int num_data = 0;
float humidity_floor = 0;
float temperature = 0;
float humidity_to_send = 0;
float temperature_to_send = 0;
float battery_to_send = 0;
float humidity_floor_print = 0;
float temperature_print = 0;

void mainSendSigfoxMessage(sigfox_msg_type msgType);
void mainSendInitialSigfoxMessage(sigfox_msg_type msgType);

void setup() {

  Serial.begin(9600);
  humidity_init();
  sht_init();
  Serial.println("[*] Hello manito!"); 
  Serial.println("[*] Starting Sigfox Configuration"); 

  rtc_init();
  sigfoxInit();
  delay(2000);
  sigfoxReadInfo();
  delay(1000);

  humidity_to_send = 50;
  temperature_to_send = 50;
  battery_to_send = 50;
  
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

  if((millis()-start_time2) > ONE_MINUTE){
    Serial.println("[*] Calculating Parameters: ");
    humidity_floor_print = get_calc_percentage_humidity();
    temperature_print = get_temperature();
    humidity_floor+= humidity_floor_print;
    temperature+=temperature_print;
    Serial.print("[*] Humidity: ");
    Serial.println(humidity_floor_print);
    Serial.print("[*] Temperature: ");
    Serial.println(temperature_print);
    start_time2 = millis();
    num_data++;
  }

  if(rtc_get_tx_flag())
  {
    Serial.println("[*] Inside Sigfox Window\r\n");    
    rtc_set_tx_flag(false);
    sigfoxInit();
    humidity_to_send = (uint16_t)humidity_floor/(float)num_data; //mean
    temperature_to_send = (uint16_t)temperature/(float)num_data; //mean

    Serial.print("[*] Humidity average: ");
    Serial.println(humidity_to_send);
    Serial.print("[*] Temperature average: ");
    Serial.println(temperature_to_send);

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

void mainSendInitialSigfoxMessage(sigfox_msg_type msgType){

  String msgBuffer_ini = "AT$SF=100000000000000000000000";
  Serial.print("[!] Message to Send: "); 
  Serial.println(msgBuffer_ini); 

  
  if(msgType == UNIDIR_MSG){
    sigfoxSendMsg(msgBuffer_ini);
  }
  else if(msgType == BIDIR_MSG)
  {
    sigfoxSendBidirMsg(msgBuffer_ini, rxBuffer);
    sigfoxParseResponse(rxBuffer);
  }
}

#ifdef TEST
int main(){
  setup();

  while(1){
    loop();
  }
}
#endif