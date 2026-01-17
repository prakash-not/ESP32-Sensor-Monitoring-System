#include "ble.h"
#include "config_manager.h"
#include "log_manager.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class RemoteCommands : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pChar) {
        String rxValue = pChar->getValue();
        if (rxValue.length() > 0) {
            uint8_t cmd = rxValue[0];
            
            if (cmd == '1') {
                sensorEnabled = true;
                log_event("SensorEnabled ");
                Serial.println("STATE: ACTIVE");
            } else if (cmd == '0') {
                sensorEnabled = false;
                log_event("SensorDisabled ");
                Serial.println("STATE: SLEEP");
            } else if (cmd == 'R') {
                log_event("SoftwareReset ");
                ESP.restart();
            } else if (cmd >= 1 && cmd <= 10) {
                samplingInterval = cmd;
                Serial.printf("Interval: %d\n", samplingInterval);
            } else {
                Serial.println("Invalid BLE Data");
            }
        }
    }
};

void ble_init() {
    BLEDevice::init("ESP32_Intern");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pChar = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
    pChar->setCallbacks(new RemoteCommands());
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
}