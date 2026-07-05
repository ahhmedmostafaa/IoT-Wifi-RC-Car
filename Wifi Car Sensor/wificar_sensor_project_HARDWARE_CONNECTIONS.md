# Hardware Connections — wificar_sensor_project.ino

Derived directly from the pin definitions (`ENA`, `ENB`, `IN_1`–`IN_4`, `LED_PIN`, `BUZZ_PIN`, `TRIG_PIN`, `ECHO_PIN`) in `wificar_sensor_project.ino`. This is the sensor-augmented version of `wificar_project.ino` — same motor/LED/buzzer wiring, plus an HC-SR04 ultrasonic sensor.

## Board
ESP8266 (NodeMCU-style board — pin macros `D0`/`D1` used alongside raw GPIO numbers for the sensor pins).

## Components Required
- ESP8266 NodeMCU board
- L298N motor driver module
- 2x DC motors (differential drive)
- 1x LED
- 1x buzzer
- 1x HC-SR04 ultrasonic distance sensor
- Motor power source (separate battery pack)
- Common ground across all modules
- **A logic-level shifter or resistor voltage divider for the sensor's `ECHO` pin (see caution below)**

## Pin Connection Table

| Code Reference | NodeMCU Pin | GPIO | Connects To | Purpose |
|---|---|---|---|---|
| `ENA` | D5 | GPIO14 | L298N `ENA` | Motor A speed (PWM) |
| `ENB` | D6 | GPIO12 | L298N `ENB` | Motor B speed (PWM) |
| `IN_1` | D8 | GPIO15 | L298N `IN1` | Motor A direction |
| `IN_2` | D7 | GPIO13 | L298N `IN2` | Motor A direction |
| `IN_3` | D4 | GPIO2 | L298N `IN3` | Motor B direction |
| `IN_4` | D3 | GPIO0 | L298N `IN4` | Motor B direction |
| `LED_PIN` | D0 | GPIO16 | LED anode (+ resistor) | Headlight / proximity-alert LED |
| `BUZZ_PIN` | D1 | GPIO5 | Buzzer signal pin | Horn / proximity-alert buzzer |
| `TRIG_PIN` | — (labeled "TX") | GPIO1 | HC-SR04 `TRIG` | Sends ultrasonic pulse |
| `ECHO_PIN` | — (labeled "RX") | GPIO3 | HC-SR04 `ECHO` **(via level shifting, see below)** | Reads pulse return time |

## L298N Motor Driver Wiring
Identical to the base project:

| L298N Terminal | Connects To |
|---|---|
| `ENA` | NodeMCU D5 |
| `ENB` | NodeMCU D6 |
| `IN1` | NodeMCU D8 |
| `IN2` | NodeMCU D7 |
| `IN3` | NodeMCU D4 |
| `IN4` | NodeMCU D3 |
| `OUT1`, `OUT2` | Motor A terminals |
| `OUT3`, `OUT4` | Motor B terminals |
| `+12V` | Battery pack positive |
| `GND` | Battery pack negative **and** NodeMCU GND |

## HC-SR04 Ultrasonic Sensor Wiring

| HC-SR04 Pin | Connects To |
|---|---|
| `VCC` | NodeMCU 5V (Vin) — HC-SR04 needs 5V, not 3.3V |
| `TRIG` | NodeMCU GPIO1 (the pin labeled "TX" on the board's silkscreen) |
| `ECHO` | **Through a voltage divider (e.g. 1kΩ + 2kΩ) or logic-level shifter** → NodeMCU GPIO3 (the pin labeled "RX") |
| `GND` | NodeMCU GND |

## ⚠️ Critical Cautions (Sensor Version Only)

1. **ECHO pin voltage mismatch.** The HC-SR04's `ECHO` output is a 5V logic signal, but ESP8266 GPIOs are only 3.3V-tolerant. Connecting `ECHO` directly to GPIO3 without a voltage divider or level shifter risks damaging the pin. This isn't shown in the code (the code only sets pin modes) — it's a physical wiring requirement that has to be handled outside the sketch.

2. **TRIG/ECHO share the hardware Serial pins.** `TRIG_PIN` (GPIO1) and `ECHO_PIN` (GPIO3) are the ESP8266's hardware UART TX/RX pins. The code's own comment confirms this ("Serial disabled because TX/RX are used for ultrasonic sensor"). Practically this means:
   - You cannot use the Serial Monitor for debugging while the sensor is wired up.
   - Uploading new code over USB still works (upload uses these pins only briefly at reset), but keep the sensor disconnected during flashing if you get upload errors.

3. **Boot-pin behavior.** As with the base project, `D3` (GPIO0) and `D4` (GPIO2) are boot-strapping pins — see the same caution in the base project's connections file.

## LED and Buzzer Wiring
Same as the base project: LED through a resistor on D0, buzzer signal on D1, both returning to GND. In this version they're also driven automatically by `checkProximity()` when an obstacle is within 3 cm, in addition to manual control.

## Power Notes
- Motor power (battery pack) must be separate from the NodeMCU's own power input; share only GND.
- The HC-SR04 needs a genuine 5V rail — powering it from the NodeMCU's 3.3V pin will not work reliably.
