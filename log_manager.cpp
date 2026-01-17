#include "log_manager.h"
#include <EEPROM.h>
#include <rom/rtc.h>

#define EEPROM_SIZE 512

void log_init() {
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    
    RESET_REASON reason = rtc_get_reset_reason(0);
    Serial.print("Boot Event Detected. Reset Reason: ");
    Serial.println(reason);

    Serial.println("Reading Stored Logs:");
    for (int i = 0; i < 50; i++) {
        char c = EEPROM.read(i);
        if (c != 0 && c != 255) Serial.print(c);
    }
    Serial.println();
}

void log_event(String msg) {
    for (int i = 0; i < msg.length() && i < 50; i++) {
        EEPROM.write(i, msg[i]);
    }
    EEPROM.commit();
}