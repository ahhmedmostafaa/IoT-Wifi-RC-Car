// Motor Driver Pins (L298N)
#define ENA   14          //  GPIO14(D5)
#define ENB   12          //  GPIO12(D6)
#define IN_1  15          //  GPIO15(D8)
#define IN_2  13          //  GPIO13(D7)
#define IN_3  2           //  GPIO2(D4)
#define IN_4  0           //  GPIO0(D3)

// Accessory pins
#define LED_PIN D0        // LED control
#define BUZZ_PIN D1       // Buzzer control

// Ultrasonic Sensor Pins (HC-SR04)
#define TRIG_PIN 1        // TX pin (GPIO1)
#define ECHO_PIN 3        // RX pin (GPIO3)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;
int speedCar = 1023;        // Current speed (0-1023)
int speed_Coeff = 5;        // Speed reduction for diagonal turns

// Ultrasonic sensor variables
unsigned long lastSensorCheck = 0;
const unsigned long sensorInterval = 100; // Check every 100ms
bool manualLedControl = false; // Track if LED is controlled manually

// WiFi credentials
const char* ssid = "WIFI Car Project";
const char* password = "YOUR_WIFI_PASSWORD_HERE"; // TODO: set your own password (min 8 chars for WPA2)

ESP8266WebServer server(80);

void setup() {
 // Configure motor pins
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 

 // Configure accessory pins
 pinMode(LED_PIN, OUTPUT);
 pinMode(BUZZ_PIN, OUTPUT);
 digitalWrite(LED_PIN, LOW); 
 digitalWrite(BUZZ_PIN, LOW); 

 // Configure ultrasonic sensor pins
 pinMode(TRIG_PIN, OUTPUT);
 pinMode(ECHO_PIN, INPUT);
 digitalWrite(TRIG_PIN, LOW);
  
 // Serial disabled because TX/RX are used for ultrasonic sensor
 Serial.begin(115200);
  
 // Create WiFi access point
 WiFi.mode(WIFI_AP);
 WiFi.softAP(ssid, password);

 IPAddress myIP = WiFi.softAPIP();
 Serial.print("AP IP address: ");
 Serial.println(myIP);
 
 // Setup web server
 server.on ( "/", HTTP_handleRoot );
 server.onNotFound ( HTTP_handleRoot );
 server.begin();    
}

// Ultrasonic sensor distance measurement function
float getDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10 microsecond pulse to trigger pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin (timeout after 30ms = ~5m max distance)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  // Calculate distance in cm (speed of sound = 343 m/s)
  // Distance = (Time × Speed) / 2
  float distance = (duration * 0.0343) / 2;
  
  return distance;
}

// Check proximity and trigger alert if object is too close
void checkProximity() {
  float distance = getDistance();
  
  // If object is within 3 cm, activate buzzer and LED
  if (distance > 0 && distance <= 3.0) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZ_PIN, HIGH);
    manualLedControl = false; // Proximity control overrides manual
  } else {
    // Turn off buzzer and LED when not in proximity
    digitalWrite(BUZZ_PIN, LOW);
    if (!manualLedControl) {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

// Beep Horn buzzer
void BeepHorn(){
  digitalWrite(BUZZ_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZ_PIN, LOW);
  delay(80);
}

// Movement functions
void goAhead(){ 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
}

void goBack(){ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
}

void goLeft(){
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
}

void goRight(){ 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
}

// Diagonal movements (one motor slower for turning) 
void goAheadRight(){
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);  
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);  
}

void goAheadLeft(){
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);  

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);  
}

void goBackRight(){ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);  

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff); 
}

void goBackLeft(){ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);  

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
}

void stopRobot(){  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
}

// LED control
void turnLightOn() {
  digitalWrite(LED_PIN, HIGH);
  manualLedControl = true; // Mark as manual control
}

void turnLightOff() {
  digitalWrite(LED_PIN, LOW);
  manualLedControl = false; // Mark as manual control off
}

void loop() {
    server.handleClient();
    
    // Check ultrasonic sensor periodically (non-blocking)
    unsigned long currentMillis = millis();
    if (currentMillis - lastSensorCheck >= sensorInterval) {
      lastSensorCheck = currentMillis;
      checkProximity();
    }
    
    command = server.arg("State");
    
    // Handle movement commands
    if (command == "F") goAhead();
    else if (command == "B") goBack();
    else if (command == "L") goLeft();
    else if (command == "R") goRight();
    else if (command == "I") goAheadRight();
    else if (command == "G") goAheadLeft();
    else if (command == "J") goBackRight();
    else if (command == "H") goBackLeft();
    else if (command == "S") stopRobot();
    
    // Handle accessory commands (led, buzzer)
    else if (command == "V") BeepHorn();
    else if (command == "W") turnLightOn();
    else if (command == "w") turnLightOff();

    // Handle speed adjustment (0-9, q for max)
    else if (command == "0") speedCar = 330;
    else if (command == "1") speedCar = 400;
    else if (command == "2") speedCar = 470;
    else if (command == "3") speedCar = 540;
    else if (command == "4") speedCar = 610;
    else if (command == "5") speedCar = 680;
    else if (command == "6") speedCar = 750;
    else if (command == "7") speedCar = 820;
    else if (command == "8") speedCar = 890;
    else if (command == "9") speedCar = 960;
    else if (command == "q") speedCar = 1023;
}

void HTTP_handleRoot(void) {
  if(server.hasArg("State")){
    // Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}