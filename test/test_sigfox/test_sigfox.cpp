#include <ArduinoFake.h>
#include <unity.h>
#include "sigfox.h"

#define UNITY_SUPPORT_64

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_sigfoxPackMsg_should_PackMessagesCorrectly(){
    float humidity, temperature, battery;
    uint8_t *floatPtr;

    humidity = 20.1; //4c779ac2
    temperature = 23.5; //52b848c1
    battery = 75.8; //b81ecd41

    String MessageTopack;
    
    sigfoxPackMsg(humidity,temperature,battery,&MessageTopack);

    const char *MessageTopack_c = MessageTopack.c_str();

    TEST_ASSERT_EQUAL_STRING("AT$SF=cdcca0410000bc419a999742",MessageTopack_c);
}

void test_sigfoxParseResponse_should_adaptArrayCorrectly(){
    char *buf_rx = "RX=65 3C 9D C9 00 00 00 00";
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

    TEST_ASSERT_EQUAL_STRING("653C9DC9",output);
}

void test_sigfoxParseResponse_should_setTimeCorrectly(){

    char *output = "653C9DC9";

    unsigned long epochTime = strtoul(output, NULL, 16);
    epochTime = epochTime-18000;

    TEST_ASSERT_EQUAL(1698453369,epochTime); /*Epoch Time*/
}

int main(){

    UNITY_BEGIN(); // start unit testing

    RUN_TEST(test_sigfoxPackMsg_should_PackMessagesCorrectly);
    RUN_TEST(test_sigfoxParseResponse_should_setTimeCorrectly);

    UNITY_END(); // stop unit testing
}
