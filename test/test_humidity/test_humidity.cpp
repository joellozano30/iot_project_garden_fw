#include <ArduinoFake.h>
#include <unity.h>
#include "humidity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_get_calc_percentage_humidity_worksCorrectly(void) {
    float percentage_humidity;
    float validation;

    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(150);
    When(Method(ArduinoFake(), map)).AlwaysReturn(12);

    percentage_humidity = get_calc_percentage_humidity();

    Verify(Method(ArduinoFake(), analogRead).Using(HUMIDITY_PIN)).Once();

    validation = percentage_humidity >= 0 && percentage_humidity <= 100;

    TEST_ASSERT(validation);
}

int main(){

    UNITY_BEGIN(); // start unit testing

    RUN_TEST(test_get_calc_percentage_humidity_worksCorrectly);

    UNITY_END(); // stop unit testing
}
