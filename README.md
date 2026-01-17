# ESP32 Sensor Monitoring & BLE Controller

## 1. Project Overview
This project is a modular firmware solution developed for an ESP32 to handle real-time DHT11 sensor data, persistent event logging, and remote control via a custom BLE interface. It emphasizes non-blocking execution and efficient power management.

## 2. File Structure & Modules
The repository is organized into distinct modules to ensure high code quality and maintainability, as per the internship requirements:

* **`project.ino`**: The main orchestrator that handles system setup and the high-level execution loop.
* **`sensor.cpp` / `sensor.h`**: Manages DHT11 sensor readings and maintains a 60-sample circular buffer.
* **`ble.cpp` / `ble.h`**: Implements a custom write-only BLE service for system commands and interval updates.
* **`log_manager.cpp` / `log_manager.h`**: Handles EEPROM storage for boot events and hardware reset reasons.
* **`config_manager.h`**: Centralizes global system states and configuration flags.

## 3. Core Technical Features

### Non-Blocking Timing
I utilized a `millis()`-based timing approach for the 1-second sampling interval. This prevents the system from "freezing" (a common issue with `delay()`), ensuring the BLE radio remains responsive to incoming commands at all times.

### Data Analytics & Buffer
The system performs real-time calculations on the sensor data:
* **Circular Buffer**: Maintains a fixed window of the last 60 temperature samples.
* **Statistics**: Computes the Minimum and Maximum values across the entire buffer.
* **Moving Average**: Calculates a 10-sample moving average to provide a smoothed data trend.

### Persistent Logging
The system uses the ESP32's EEPROM to ensure data survives power cycles:
* **Reset Detection**: On every boot, the system identifies the hardware reset reason (e.g., Power-on vs. Software Reset).
* **Event Retrieval**: Historical logs stored in memory are printed to the Serial Monitor upon startup for system auditing.

## 4. Hardware Configuration
* **Microcontroller**: ESP32 DevKit V1
* **Sensor**: DHT11
* **Wiring**: 
    * VCC to 3.3V
    * GND to GND
    * Data Pin to **GPIO 4**
* **Setup Photo**: See `hardware_setup.jpg` in the root folder for the physical wiring diagram.

## 5. Usage & Testing
To interact with the device, use a BLE scanner like **nRF Connect**:

1.  Connect to the device named **"ESP32_Intern"**.
2.  Locate the custom service (`4fafc201...`) and characteristic.
3.  Send the following commands (Text/UTF-8):
    * `1`: Start sensor sampling (**ACTIVE** state).
    * `0`: Stop sampling and enter low-power mode (**SLEEP** state).
    * `1-10`: Set the sampling interval in seconds (e.g., send `5` for 5-second intervals).
    * `R`: Trigger a software reset to verify persistent logging.

---
**Note**: Detailed execution logs can be found in `serial_logs.txt`.
