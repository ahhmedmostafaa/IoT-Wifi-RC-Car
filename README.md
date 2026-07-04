# IoT RC Car — WiFi & Bluetooth Controlled, with Obstacle Sensing

An ESP8266/Arduino-based remote-controlled car with three evolving control implementations: WiFi (via built-in web server), Bluetooth (serial commands), and a sensor-augmented WiFi version with ultrasonic obstacle detection.

## How it works
The car uses an **L298N motor driver** to control two DC motors independently (differential drive), enabling forward, backward, left, right, and diagonal movement.

- **WiFi versions (ESP8266):** the board creates its own WiFi access point and hosts a lightweight web server. Movement commands are sent as HTTP requests from any browser/phone connected to the car's network — no app installation needed.
- **Bluetooth version:** an Arduino + Bluetooth module receives single-character serial commands (`F`, `B`, `L`, `R`, etc.) typically sent from a Bluetooth RC-car controller app, with adjustable speed levels and an electronic braking feature.

## Project Versions

| Folder | Description |
|---|---|
| `wificar_project/` | Base version — WiFi AP + web server, LED and buzzer control (horn) alongside driving |
| `wificar_sensor_project/` | Adds an **HC-SR04 ultrasonic sensor** for real-time obstacle distance sensing |
| `Bluetooth_Car/` | Alternative control scheme over Bluetooth serial instead of WiFi |

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
1. Open the relevant `.ino` file in the Arduino IDE
2. Install board support for ESP8266 (WiFi versions) via Boards Manager
3. Wire the components as described in the accompanying project documentation (PDF)
4. Upload the sketch
5. **WiFi versions:** connect to the car's WiFi network (`WIFI Car Project`) and open its IP in a browser
6. **Bluetooth version:** pair via a Bluetooth serial terminal or RC-car controller app

## Roadmap
- [ ] Merge the WiFi variants into one configurable sketch (feature flags)
- [ ] Add a short demo video/GIF of the car in action
- [ ] Add the wiring diagram as an image directly in this README
