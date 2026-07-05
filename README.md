# IoT RC Car — WiFi & Bluetooth Controlled, with Obstacle Sensing

An ESP8266/Arduino-based remote-controlled car with three evolving control implementations: WiFi (via built-in web server), Bluetooth (serial commands), and a sensor-augmented WiFi version with ultrasonic obstacle detection.

## How it works
The car uses an **L298N motor driver** to control two DC motors independently (differential drive), enabling forward, backward, left, right, and diagonal movement.

- **WiFi versions (ESP8266):** the board creates its own WiFi access point and hosts a lightweight web server. Movement commands are sent as HTTP requests from any browser/phone connected to the car's network — no app installation needed.
- **Bluetooth version:** an Arduino + Bluetooth module receives single-character serial commands (`F`, `B`, `L`, `R`, etc.) typically sent from a Bluetooth RC-car controller app, with adjustable speed levels and an electronic braking feature.

## Project Versions

| Folder | File | Description |
|---|---|---|
| [`Wifi Car/`](./Wifi%20Car) | `wificar_project.ino` | Base version — WiFi AP + web server, LED and buzzer control (horn) alongside driving |
| [`Wifi Car Sensor/`](./Wifi%20Car%20Sensor) | `wificar_sensor_project.ino` | Adds an **HC-SR04 ultrasonic sensor** for real-time obstacle distance sensing |
| [`Bluetooth Car/`](./Bluetooth%20Car) | `Bluetooth_Car.ino` | Alternative control scheme over Bluetooth serial instead of WiFi |

Each folder also contains a `HARDWARE_CONNECTIONS.md` with the full wiring/pinout for that specific version, plus a reference photo.

## Hardware Used
- ESP8266 (WiFi versions) / Arduino + Bluetooth module (Bluetooth version)
- L298N Motor Driver
- DC Motors (x2 or x4, differential drive)
- HC-SR04 Ultrasonic Sensor (sensor version only)
- LED + Buzzer (accessory version)

## Tech Stack
- C++ (Arduino framework)
- ESP8266WiFi / ESP8266WebServer libraries

## How to Run
1. Open the relevant `.ino` file (inside its version folder) in the Arduino IDE
2. Install board support for ESP8266 (WiFi versions) via Boards Manager
3. Wire the components as described in that version's `HARDWARE_CONNECTIONS.md`
4. Upload the sketch
5. **WiFi versions:** connect to the car's WiFi network (`WIFI Car Project`) and open its IP in a browser
6. **Bluetooth version:** pair via a Bluetooth serial terminal or RC-car controller app

## Repository Structure
```
IoT-Wifi-RC-Car/
├── README.md
├── .gitignore
│
├── Bluetooth Car/
│   ├── Bluetooth_Car.ino
│   ├── Bluetooth_Car_HARDWARE_CONNECTIONS.md
│   └── bluetooth-rc-car-arduino-program.jpg
│
├── Wifi Car/
│   ├── wificar_project.ino
│   ├── wificar_project_HARDWARE_CONNECTIONS.md
│   └── Wifi-controlled-car.jpg
│
└── Wifi Car Sensor/
    ├── wificar_sensor_project.ino
    └── wificar_sensor_project_HARDWARE_CONNECTIONS.md
```
