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

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;
int speedCar = 1023;        // Current speed (0-1023)
int speed_Coeff = 5;        // Speed reduction for diagonal turns

// WiFi credentials
const char* ssid = "WIFI Car Project";
const char* password = "ahmed123"; 

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
}

void turnLightOff() {
  digitalWrite(LED_PIN, LOW);
}

void loop() {
    server.handleClient();
    
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
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}
