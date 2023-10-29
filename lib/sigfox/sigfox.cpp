#include "sigfox.h"

uint8_t DwnRecFlag = false;
char sigfoxRxBuffer[50] = {0};
char sigfoxID[51] = {0};
char sigfoxPAC[51] = {0};

#ifndef TEST
SoftwareSerial mySerial(0, 1); // RX, TX
#endif

void sigfoxInit(void)
{
    #ifndef TEST
      mySerial.begin(9600);
    #else
      Serial.begin(9600);
    #endif
    pinMode(SIGFOX_ENABLE, OUTPUT);
    digitalWrite(SIGFOX_ENABLE, LOW);
    sigfoxChangeToRegion4();
}

void sigfoxChangeToRegion4(void)
{
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    Serial.print("AT$DR=922300000");
    Serial.print("\r\n");
    Serial.print("ATS400=<00000000><F0000000><0000001F>,63");
    Serial.print("\r\n");
    Serial.print("AT$WR");
    Serial.print("\r\n");
    Serial.print("AT$RC");
    Serial.print("\r\n");

    delay(1000);
    
    Serial.println("Cambiado zona 4");
    digitalWrite(SIGFOX_ENABLE, LOW);
}

void sigfoxReadInfo(void)
{
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);
    sigfoxSendATCommand("AT");
    sigfoxSendATCommand("AT$I=10");
    strcpy(sigfoxID, sigfoxRxBuffer);

    sigfoxSendATCommand("AT$I=11");
    strcpy(sigfoxPAC, sigfoxRxBuffer);

    sigfoxSendATCommand("AT$RC");
    digitalWrite(SIGFOX_ENABLE, LOW);

    Serial.println("[!] Showing Sigfox Parameters:");
    Serial.print("ID: ");
    Serial.println(sigfoxID);
    Serial.print("PAC: ");
    Serial.println(sigfoxPAC);

}

void sigfoxSendATCommand(char* comandoAT)
{
  unsigned long x=0;

  while( Serial.available() > 0) Serial.read();
    x = 0;
  memset(sigfoxRxBuffer, '\0',sizeof(sigfoxRxBuffer)); 
  Serial.print(comandoAT);
  Serial.print("\r\n");
  while(true)
  {
    if(Serial.available() != 0)
    {   
      sigfoxRxBuffer[x] = Serial.read();
      x++;
      if (strstr(sigfoxRxBuffer, "\n") != NULL)
      {
        break;
      }
    }
  }
}

void sigfoxPackMsg(float humidity, float temperature, float battery, String *dataMsg)
{
    uint8_t *floatPtr;
    String newHumByte, newTempByte, newBatByte;

    *dataMsg = "AT$SF=";

    int idx = 0;

    floatPtr = (uint8_t *)&humidity;
    #ifdef GPS_PACK_MSG_DEBUG
    Serial.println("[*] Generating Latitude values");
    #endif
    for(idx = 0; idx < 4; idx++)
    {
        newHumByte = String(*(floatPtr + idx), HEX);
        if(*(floatPtr + idx) <16)
            newHumByte = "0" + newHumByte;
        #ifdef GPS_PACK_MSG_DEBUG
        Serial.print("[*] Byte number "); Serial.print(idx); Serial.print(" value: "); Serial.println(newLatByte);
        #endif
        *dataMsg += newHumByte;
    }

    floatPtr = (uint8_t *)&temperature;
    #ifdef GPS_PACK_MSG_DEBUG
    Serial.println("[*] Generating Longitude values");
    #endif
    for(idx = 0; idx < 4; idx++)
    {
        newTempByte = String(*(floatPtr + idx), HEX);
        if(*(floatPtr + idx) < 16)
            newTempByte = "0" + newTempByte;
        #ifdef GPS_PACK_MSG_DEBUG
        Serial.print("[*] Byte number "); Serial.print(idx); Serial.print(" value: "); Serial.println(newLatByte);
        #endif
        *dataMsg += newTempByte;
    }

    floatPtr = (uint8_t *)&battery;
    #ifdef GPS_PACK_MSG_DEBUG
    Serial.println("[*] Generating Longitude values");
    #endif
    for(idx = 0; idx < 4; idx++)
    {
        newBatByte = String(*(floatPtr + idx), HEX);
        if(*(floatPtr + idx) < 16)
            newBatByte = "0" + newBatByte;
        #ifdef GPS_PACK_MSG_DEBUG
        Serial.print("[*] Byte number "); Serial.print(idx); Serial.print(" value: "); Serial.println(newLatByte);
        #endif
        *dataMsg += newBatByte;
    }

    Serial.print("[*] Sigfox message: "); Serial.println(*dataMsg);
}

void sigfoxSendMsg(String buf_tx)
{
    //Adding line break
    buf_tx += "\n";

    //******************************
    //Enable Sigfox Module
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    //Channel reset to ensure correct frequency
    Serial.print("AT$RC\n");
    
    //******************************
    //Sending data on Sigfox
    Serial.print(buf_tx);

    #ifdef SIGFOX_DEBUG
        #ifndef TEST
        Serial.print(buf_tx);
        #endif
    #endif
    delay(3000);

    //disable sigfox module
    digitalWrite(SIGFOX_ENABLE, LOW);
}

void sigfoxSendBidirMsg(String buf_tx, char *buf_rx){

    //Add downlink indicator and line break \n
    buf_tx += ",1\n";

    //*****************************************
    //Enable Sigfox Module
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    while(Serial.available() > 0)
        Serial.read();

    //Reset channel to ensure correct frequency
    Serial.print("AT$RC\n");
    
    delay(1000);
    //*****************************************
    //Sending data to Sigfox
    Serial.print(buf_tx);

    sigfoxReadResponse(buf_rx);

    digitalWrite(SIGFOX_ENABLE, LOW);

    Serial.println("Response: ");
    Serial.println(buf_rx);
}

void sigfoxReadResponse(char *buf_rx){
  int idx = 0;
  uint32_t start_time = millis();

  while(true)
  {
    if(Serial.available() > 0)
    {
      buf_rx[idx] = Serial.read();
      idx++;
    }
    if((millis() - start_time) > FIFTY_SECONDS)
    {
      break;
    }
        
   delay(10);
  }
}

void sigfoxParseResponse(char* buf_rx){
  char output[9];
  int inputIndex = 0;
  int outputIndex = 0;

   if(strstr(buf_rx, "RX=") == NULL)
  {
      Serial.print("[!] No valid downlink frame found, returning null\r\n");
      return;
  }

  sx_set_dwnrec_flag(true);

  while (buf_rx[inputIndex] != '\0' && buf_rx[inputIndex] != '=') {
    inputIndex++;
  }

  inputIndex++;

  while (buf_rx[inputIndex] != '\0' && outputIndex < 8) {
    if (buf_rx[inputIndex] != ' ') {
      output[outputIndex] = buf_rx[inputIndex];
      outputIndex++;
    }
    inputIndex++;
  }

  output[outputIndex] = '\0';

  unsigned long epochTime = strtoul(output, NULL, 16);
  epochTime = epochTime-18000;

  Serial.print("Epoch time: ");
  Serial.println(epochTime);

  #ifndef TEST
  tmElements_t currentTime;
  breakTime(epochTime, currentTime);

  // Imprimir la hora, minutos y segundos
  Serial.print("Hora recibida: ");
  Serial.print(currentTime.Hour);
  Serial.print(":");
  Serial.print(currentTime.Minute);
  Serial.print(":");
  Serial.println(currentTime.Second);

  setTime(currentTime.Hour,currentTime.Minute,currentTime.Second,1,1,2020);
  #endif

}

void sigfox_enter_sleep_mode(void){

    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);
    sigfoxSendATCommand("AT$P=1");
    delay(1000);
}


void sx_set_dwnrec_flag(uint8_t val)
{
    DwnRecFlag = val;
}

uint8_t sx_get_dwnrec_flag(void)
{
    return DwnRecFlag;
}