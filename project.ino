#include "config_manager.h"
#include "sensor.h"
#include "ble.h"
#include "log_manager.h"

bool sensorEnabled = false;
int samplingInterval = 1;
unsigned long lastTime = 0;

void setup() {
    Serial.begin(115200);
    log_init();
    sensor_init();
    ble_init();
}

void loop() {
    if (sensorEnabled) {
        if (millis() - lastTime >= (samplingInterval * 1000)) {
            lastTime = millis();
            sensor_process();
        }
    } else {
        delay(100); 
    }
}