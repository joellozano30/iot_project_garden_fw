#include "sigfox.h"

char sigfoxRxBuffer[50] = {0};
char sigfoxID[51] = {0};
char sigfoxPAC[51] = {0};

HardwareSerial MySerial(2); // definir un Serial para UART1
const int MySerialRX = 16;
const int MySerialTX = 17;

void sigfoxInit(void)
{
    MySerial.begin(9600, SERIAL_8N1, MySerialRX, MySerialTX);
    //Serial1.begin(9600, SERIAL_8N1);
    pinMode(SIGFOX_ENABLE, OUTPUT);
    digitalWrite(SIGFOX_ENABLE, LOW);
    sigfoxChangeToRegion4();
}

void sigfoxChangeToRegion4(void)
{
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    MySerial.print("AT$DR=922300000");
    MySerial.print("\r\n");
    MySerial.print("ATS400=<00000000><F0000000><0000001F>,63");
    MySerial.print("\r\n");
    MySerial.print("AT$WR");
    MySerial.print("\r\n");
    MySerial.print("AT$RC");
    MySerial.print("\r\n");

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

  while( Serial1.available() > 0) Serial1.read();
    x = 0;
  memset(sigfoxRxBuffer, '\0',sizeof(sigfoxRxBuffer)); 

  Serial.println(comandoAT);

  Serial2.print(comandoAT);
  Serial2.print("\r\n");
  while(true)
  {
    Serial.println("Reading ... ");
    if(Serial2.available() != 0)
    {   
      sigfoxRxBuffer[x] = Serial2.read();
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
    Serial2.print("AT$RC\n");
    
    //******************************
    //Sending data on Sigfox
    Serial2.print(buf_tx);

    #ifdef SIGFOX_DEBUG
        #ifndef TEST
        Serial.print(buf_tx);
        #endif
    #endif
    delay(3000);

    //disable sigfox module
    digitalWrite(SIGFOX_ENABLE, LOW);
}

void sigfoxSendBidirMsg(String buf_tx, String buf_rx){

    //Add downlink indicator and line break \n
    buf_tx += ",1\n";

    //*****************************************
    //Enable Sigfox Module
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    //Reset channel to ensure correct frequency
    Serial2.print("AT$RC\n");
    
    //*****************************************
    //Sending data to Sigfox
    Serial2.print(buf_tx);

    sigfoxReadResponse(buf_rx);

    digitalWrite(SIGFOX_ENABLE, LOW);
}

void sigfoxReadResponse(String buf_rx){
  int idx = 0;
  uint32_t start_time = millis();

  while(true)
  {
    if(Serial1.available() > 0)
    {
      buf_rx[idx] = Serial1.read();
      idx++;
    }
    if((millis() - start_time) > SEVENTY_FIVE_SECONDS)
    {
      break;
    }
        
   delay(10);
  }
}

// void sigfoxSendMsg(String sigfoxTxBuffer)
// {
//     // Adding newline to message
//     sigfoxTxBuffer += "\n";

//     //**************************
//     // Enabling Sigfox module
//     digitalWrite(SIGFOX_ENABLE, HIGH);
//     delay(1000);

//     // Reset channel to ensure that message is sent in the right frequency
//     #ifdef SIGFOX_DEBUG
//         Serial.print("AT$RC\n");
//     #endif
//     Serial1.print("AT$RC\n");

//     //**************************
//     // Sending the message via Sigfox
//     #ifdef SIGFOX_DEBUG
//         Serial.print(sigfoxTxBuffer);
//     #endif
//     Serial1.print(sigfoxTxBuffer);
//     delay(3000);
//     // Disabling Sigfox module
//     digitalWrite(SIGFOX_ENABLE, LOW);

//     // Flushing the buffer
//     while(Serial1.available() > 0)
//         Serial1.read();
// }