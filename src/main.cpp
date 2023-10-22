#include <Arduino.h>
//#include <SoftwareSerial.h>
#include "sigfox.h"
#include "humidity.h"
#include "temperature.h"

//SoftwareSerial mySerial(0, 1); // RX, TX

#define RX_BUFFER_SIZE 100
#define FIVE_SECONDS 5000

String msgBuffer = "";
char rxBuffer[RX_BUFFER_SIZE] = {'\0'};
uint8_t start_time;
int num_data = 0;
float humidity_floor = 0;
float temperature = 0;
float humidity_to_send = 0;
float temperature_to_send = 0;
float battery_to_send = 0;
int txFlag = 1; //Waiting time to send

void mainSendSigfoxMessage(sigfox_msg_type msgType);

void setup() {

  Serial.begin(9600);
  //humidity_init();
  //temperature_init();
  Serial.println("[*] Hello!"); 
  Serial.println("[*] Starting Sigfox Configuration"); 

  sigfoxInit();
  delay(2000);
  sigfoxReadInfo();
  delay(1000);

  humidity_to_send = 45.75;
  temperature_to_send = 26.73;
  battery_to_send = 80;

  Serial.println("[*] Starting Pack Message"); 
  sigfoxPackMsg(humidity_to_send, temperature_to_send, battery_to_send, &msgBuffer);
  Serial.println("[*] Sending Message"); 
  sigfoxSendMsg(msgBuffer);

  // sigfoxSendATCommand("AT");
  // sigfoxSendATCommand("AT$I=10");
  // strcpy(ID,RespuestaSigfox);

  // delay(1000);
  // digitalWrite(SIGFOX_ENABLE, LOW);
}

void loop() {

  // while((millis()-start_time)<FIVE_SECONDS){
  //   humidity_floor+=get_calc_percentage_humidity();
  //   //temperature+=get_temperature();
  //   start_time = millis();
  //   num_data++;
  // }

  // if(txFlag){
  //   humidity_to_send = humidity_floor/num_data; //mean
  //   temperature_to_send = temperature/num_data; //mean
  //   mainSendSigfoxMessage(UNIDIR_MSG);
  //   num_data = 0;
  //   txFlag = false;
  // }
  
}

void mainSendSigfoxMessage(sigfox_msg_type msgType){

  sigfoxPackMsg(humidity_to_send, temperature_to_send, battery_to_send, &msgBuffer);
  Serial.print("[!] Message to Send: "); 
  Serial.print(msgBuffer); 
  
  if(msgType == UNIDIR_MSG){
    sigfoxSendMsg(msgBuffer);
  }
  else if(msgType == BIDIR_MSG)
  {
    sigfoxSendBidirMsg(msgBuffer, rxBuffer);
    sigfoxParseResponse(rxBuffer);
  }

}