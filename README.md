# ESP32 Sensor Monitoring & BLE Controller

## 1. Project Overview
This project is a modular firmware solution for the ESP32 designed to handle real-time DHT11 sensor data, persistent event logging, and remote management via a custom BLE interface. The system focuses on non-blocking execution using `millis()` and power-efficient operation.

## 2. Hardware Setup
To verify the physical implementation, the circuit is built on a breadboard with the following configuration:

![Hardware Setup](hardware_setup (1).jpg)

* **Microcontroller**: ESP32 DevKit V1
* **Sensor**: DHT11
* **Wiring**: 
    * **VCC**: 3.3V
    * **GND**: GND
    * **Data Pin**: GPIO 4

## 3. System Architecture
The firmware follows a strictly modular structure to ensure maintainability and readability, as required for the internship assignment:

* **`project.ino`**: The main entry point that handles system initialization and the high-level non-blocking loop.
* **`sensor.cpp / .h`**: Manages DHT11 sensor readings and maintains a 60-sample circular buffer.
* **`ble.cpp / .h`**: Implements a custom write-only BLE service for system commands and interval updates.
* **`log_manager.cpp / .h`**: Handles EEPROM storage for persistent event logging and hardware reset reason detection.
* **`config_manager.h`**: A central configuration file for sharing global states and system flags across modules.

## 4. Technical Implementation

### Non-Blocking Timing
Instead of using `delay()`, the system utilizes a `millis()`-based approach for the 1-second sampling interval. This ensures that the BLE stack remains responsive and the system can process incoming commands immediately without latency.

### Data Handling & Statistics
The system maintains a fixed window of the last 60 samples to provide accurate historical data:
* **Circular Buffer**: Stores the last 60 temperature readings.
* **Min/Max**: Calculated across the entire 60-sample buffer.
* **Moving Average**: Uses a window of the last 10 samples to smooth out data trends.

### Persistent Memory
By utilizing the ESP32's EEPROM, the system ensures that logs survive power cycles:
* **Reset Detection**: Identifies the hardware reset reason (e.g., Software Reset vs. Power-on) on every boot.
* **Log Retrieval**: Historical data stored in memory is printed to the Serial Monitor upon startup.

## 5. Usage & Testing
Use **nRF Connect** or a similar BLE scanner to interact with the device:

1. **Connect** to the device named **"ESP32_Intern"**.
2. **Locate** the custom characteristic (`beb5483e...`) and use the **Write** command:
    * Send `1`: Start sampling (**ACTIVE** state).
    * Send `0`: Stop sampling and enter **SLEEP** mode.
    * Send `1-10`: Set the sampling interval in seconds (e.g., send `5` for a 5-second delay).
    * Send `R`: Trigger a software reset to verify the boot logs.

---
**Note**: For detailed execution data, refer to the included `serial_logs.txt` file.
