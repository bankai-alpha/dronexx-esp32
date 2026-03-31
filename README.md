# DroneX ESP32 Flight Controller 🧠

Modular C++ firmware for the ESP32 microcontroller, acting as a high-speed Wi-Fi Access Point (SoftAP) and UDP receiver for custom drone builds. 

## Features

* **Zero-Latency UDP:** Bypasses TCP overhead for real-time flight command processing.
* **Hardware Failsafe (Watchdog):** Instantly cuts motor power if the connection to the mobile app is lost for more than 2 seconds.
* **Dynamic Status LED:** Non-blocking visual feedback using the built-in LED (Chaotic strobe = Disconnected, Slow breathing pulse = Connected & Armed).
* **Object-Oriented Design:** Highly modular tabbed architecture separating network logic, hardware controls, and configuration.
* **Core 3.x PWM Native:** Uses the modern, simplified ESP32 Core 3.x `ledc` API for motor and LED pulsing.

## Hardware Requirements

* **Microcontroller:** ESP32 Development Board (e.g., NodeMCU-32S, ESP32-WROOM)
* **Motors:** Coreless brushed motors (Requires N-channel MOSFETs like SI2302 on the PWM pins; do *not* power motors directly from GPIO pins).

## Software Architecture

The code is split into distinct modules for maintainability:

* `DroneConfig.h`: The single source of truth for Wi-Fi credentials, ports, and hardware pins.
* `DroneNetwork.h`: Handles the SoftAP setup, the two-way UDP handshake, and parses incoming `pitch,roll,yaw,throttle` commands.
* `DroneStatusLED.h`: A non-blocking, timer-based engine for visual system feedback.
* `dronexx_esp32.ino`: The main orchestrator loop.

## Getting Started

### Prerequisites
* **Arduino IDE** (v2.0+ recommended).
* **ESP32 Board Manager Core v3.x.x** (CRITICAL: Version 2.x will fail to compile due to changes in the PWM API).

### Installation & Flashing

1. Open `dronexx_esp32.ino` in the Arduino IDE.
2. (Optional) Open the `DroneConfig.h` tab to customize your network SSID and Password.
3. Select your specific ESP32 board and COM port.
4. Click **Upload**.

### System States (LED Guide)
* **Erratic Fast Blinking:** System booted, broadcasting Wi-Fi, waiting for app connection.
* **Slow Breathing Pulse:** Handshake successful, app connected, actively receiving flight commands.
* **Sudden Switch to Fast Blinking:** Watchdog triggered (Signal lost). Motors killed. 

## Upcoming Features
* MPU6050 Gyroscope integration for auto-leveling.
* PID Controller implementation for the Motor Mixer.
