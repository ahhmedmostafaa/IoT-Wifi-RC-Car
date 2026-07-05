# Hardware Connections — Bluetooth_Car.ino

Derived directly from the pin definitions and logic in `Bluetooth_Car.ino`.

## Board
Arduino Uno/Nano (pins 5, 6, 10, 11, 13 and hardware Serial on pins 0/1 match the standard Uno/Nano pinout).

## Components Required
- Arduino Uno or Nano
- L298N motor driver module
- 2x DC motors (differential drive)
- HC-05 / HC-06 Bluetooth serial module
- 1x LED (indicator)
- Motor power source (6–12V battery pack, matched to your motors)
- Common ground between Arduino, L298N, and Bluetooth module

## Pin Connection Table

| Code Reference | Arduino Pin | Connects To | Purpose |
|---|---|---|---|
| `in1` | D5 (PWM) | L298N `IN1` | Motor A direction/speed |
| `in2` | D6 (PWM) | L298N `IN2` | Motor A direction/speed |
| `in3` | D10 (PWM) | L298N `IN3` | Motor B direction/speed |
| `in4` | D11 (PWM) | L298N `IN4` | Motor B direction/speed |
| `LED` | D13 | LED anode (+ resistor) | Status indicator |
| — | D0 (RX) | Bluetooth module `TXD` | Receive commands from phone |
| — | D1 (TX) | Bluetooth module `RXD` | (not used by this sketch, wire for completeness) |

`Serial.begin(9600)` uses the **hardware** RX/TX pins (0/1), so the Bluetooth module must be wired to those pins, not to a `SoftwareSerial` pair.

## L298N Motor Driver Wiring

| L298N Terminal | Connects To |
|---|---|
| `IN1` | Arduino D5 |
| `IN2` | Arduino D6 |
| `IN3` | Arduino D10 |
| `IN4` | Arduino D11 |
| `ENA` | Jumper to 5V (or remove jumper and tie to 5V) — **not driven by code**, so it must be permanently enabled |
| `ENB` | Jumper to 5V (or remove jumper and tie to 5V) — same as above |
| `OUT1`, `OUT2` | Motor A terminals |
| `OUT3`, `OUT4` | Motor B terminals |
| `+12V` (motor power in) | Battery pack positive |
| `GND` | Battery pack negative **and** Arduino GND (common ground, required) |
| `5V` (onboard regulator out) | Can power Arduino 5V pin *only if* the L298N's onboard regulator is enabled and the input voltage is within its regulator range — otherwise power the Arduino separately |

> ⚠️ The code drives speed via `analogWrite()` directly on `IN1–IN4`, not on `ENA`/`ENB`. This means `ENA`/`ENB` must stay physically jumpered/tied HIGH on the L298N board, or the motors will never turn regardless of what the code does.

## Bluetooth Module Wiring (HC-05/HC-06)

| Module Pin | Connects To |
|---|---|
| `VCC` | Arduino 5V |
| `GND` | Arduino GND |
| `TXD` | Arduino RX (D0) |
| `RXD` | Arduino TX (D1) — **through a voltage divider** if the module is 3.3V logic (most HC-05/06 boards are) |

> ⚠️ Most HC-05/HC-06 modules have 3.3V logic on `RXD`. Feeding it 5V directly from the Arduino's TX pin can damage the module over time. A simple 1kΩ/2kΩ resistor voltage divider on that line is standard practice.

## LED Wiring
- LED anode → Arduino D13 (through a ~220Ω–330Ω current-limiting resistor)
- LED cathode → GND

## Power Notes
- Motor power (battery pack) is separate from Arduino logic power; both must share a common GND.
- Do not power the motors directly from the Arduino's 5V pin — use the L298N's dedicated motor supply input.
