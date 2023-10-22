// #include "temperature.h"

// void temperature_init(){
//     dht_sensor.begin();
// }

// float get_temperature(){
//     float temp=dht_sensor.readTemperature(); //Envinromental
//     if (isnan(temp)) {
//         Serial.println(F("Failed to read temperature!"));
//         return 0;
//     }
//     return temp;
// }
// float get_humidity(){
//     float hum=dht_sensor.readHumidity(); //Environmental
//     if (isnan(hum)) {
//         Serial.println(F("Failed to read humidity!"));
//         return 0;
//     }
//     return hum;
// }