#include <ArduinoFake.h>
#include <unity.h>
#include "rtc.h"

void setUp(void) {
    // set stuff up here
    rtc_init();
}

void tearDown(void) {
    // clean stuff up here
}


void test_rtc_check_if_time_to_send_should_evaluateCorrectly_whenIsFreezeTimeToSend(void) {
    uint8_t itsTime;

    itsTime = rtc_check_if_time_to_send();

    TEST_ASSERT_EQUAL(true, itsTime);
}

void test_rtc_check_if_time_to_send_should_evaluateCorrectly_whenIsTimeToSend(void) {
    uint8_t itsTime;

    rtc_handler();
    itsTime = rtc_get_tx_flag();

    TEST_ASSERT_EQUAL(true, itsTime);
}


int main(){

    UNITY_BEGIN(); // start unit testing

    RUN_TEST(test_rtc_check_if_time_to_send_should_evaluateCorrectly_whenIsFreezeTimeToSend);
    RUN_TEST(test_rtc_check_if_time_to_send_should_evaluateCorrectly_whenIsTimeToSend);

    UNITY_END(); // stop unit testing
}
