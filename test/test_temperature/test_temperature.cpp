#include <ArduinoFake.h>
#include <unity.h>
#include "temperature.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_function(void) {
    // include unit to test 
    TEST_IGNORE();
}

int main(){

    UNITY_BEGIN(); // start unit testing

    RUN_TEST(test_function);

    UNITY_END(); // stop unit testing
}
