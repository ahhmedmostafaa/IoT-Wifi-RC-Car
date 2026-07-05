# Hardware Connections — wificar_project.ino

Derived directly from the pin definitions (`ENA`, `ENB`, `IN_1`–`IN_4`, `LED_PIN`, `BUZZ_PIN`) and includes of `ESP8266WiFi.h` in `wificar_project.ino`.

## Board
ESP8266 (NodeMCU-style board — pin macros `D0`–`D8` used, confirmed by the `#include <ESP8266WiFi.h>` / `<ESP8266WebServer.h>` includes).

## Components Required
- ESP8266 NodeMCU board
- L298N motor driver module
- 2x DC motors (differential drive)
- 1x LED
- 1x buzzer
- Motor power source (battery pack matched to your motors, separate from the ESP8266's 5V/3.3V supply)
- Common ground between NodeMCU and L298N

## Pin Connection Table

| Code Reference | NodeMCU Pin | GPIO | Connects To | Purpose |
|---|---|---|---|---|
| `ENA` | D5 | GPIO14 | L298N `ENA` | Motor A speed (PWM) |
| `ENB` | D6 | GPIO12 | L298N `ENB` | Motor B speed (PWM) |
| `IN_1` | D8 | GPIO15 | L298N `IN1` | Motor A direction |
| `IN_2` | D7 | GPIO13 | L298N `IN2` | Motor A direction |
| `IN_3` | D4 | GPIO2 | L298N `IN3` | Motor B direction |
| `IN_4` | D3 | GPIO0 | L298N `IN4` | Motor B direction |
| `LED_PIN` | D0 | GPIO16 | LED anode (+ resistor) | Headlight/status LED |
| `BUZZ_PIN` | D1 | GPIO5 | Buzzer signal pin | Horn |

## L298N Motor Driver Wiring

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
| `+12V` (motor power in) | Battery pack positive |
| `GND` | Battery pack negative **and** NodeMCU GND (common ground, required) |

## LED and Buzzer Wiring
- LED anode → D0, through a ~220Ω–330Ω resistor, cathode → GND
- Buzzer signal → D1, buzzer GND → GND (use an active buzzer, since the code only toggles the pin HIGH/LOW rather than generating a tone)

## ⚠️ Boot-Pin Caution (ESP8266-Specific)
Two of the motor pins used here are ESP8266 **boot-strapping pins**:
- `D3` = **GPIO0** — must be HIGH at boot to run normal firmware (pulling it LOW at boot triggers flash mode)
- `D4` = **GPIO2** — must be HIGH at boot

Since these are wired to the L298N `IN_4`/`IN_3` inputs, whatever state the L298N inputs float to at power-up can affect whether the board boots normally. In practice this usually works because the L298N inputs don't hold the GPIO down hard, but if you see erratic boot behavior, check these two connections first.

## Power Notes
- Motor power (battery pack) must be separate from the NodeMCU's own power input; share only GND.
- The NodeMCU should be powered via its USB/5V pin or a regulated 5V/3.3V source — do not power it from the L298N's motor supply rail directly unless a proper regulator stage is in between.
