#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID" // Replace with your Blynk Template ID
#define BLYNK_TEMPLATE_NAME "ESP32 "
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"  // Replace with your Blynk Auth Token

#define ENA   13          // Enable/speed motors Right        GPIO13
#define ENB   12          // Enable/speed motors Left         GPIO12
#define IN_1  14          // L298N in1 motors Right           GPIO14
#define IN_2  27          // L298N in2 motors Right           GPIO27
#define IN_3  26          // L298N in3 motors Left            GPIO26
#define IN_4  25          // L298N in4 motors Left            GPIO25


#include<Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "YOUR_WIFI_SSID";         // Replace with your WiFi SSID
char pass[] = "YOUR_WIFI_PASSWORD";     // Replace with your WiFi password    

int speedCar = 150;      // 0 - 255               


void setup() {
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);  
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  // Setup PWM for speed control (ESP32 requires PWM to be set on channels)
  ledcSetup(0, 5000, 8);   // Channel 0, 5kHz, 8-bit resolution
  ledcSetup(1, 5000, 8);   // Channel 1, 5kHz, 8-bit resolution
  ledcAttachPin(ENA, 0);   // Attach ENA to PWM channel 0
  ledcAttachPin(ENB, 1);   // Attach ENB to PWM channel 1

  Serial.begin(9600);
  
  // Connecting to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void goAhead() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  ledcWrite(0, speedCar);  // Set speed

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  ledcWrite(1, speedCar);  // Set speed
  Serial.println("FORWARD");
}

void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  ledcWrite(0, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  ledcWrite(1, speedCar);
  Serial.println("BACKWARD");
}

void goRight() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  ledcWrite(0, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  ledcWrite(1, speedCar);
  Serial.println("RIGHT");
}

void goLeft() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  ledcWrite(0, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  ledcWrite(1, speedCar);
  Serial.println("LEFT");
}

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  ledcWrite(0, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  ledcWrite(1, 0);
  Serial.println("STOP");
}

// Blynk virtual pin functions to control the robot
BLYNK_WRITE(V0) {  // Forward button on the app
  int value = param.asInt();
  if (value == 1) goAhead();  // Move forward
  else stopRobot();  // Stop
}

BLYNK_WRITE(V3) {  // Backward button on the app
  int value = param.asInt();
  if (value == 1) goBack();  // Move backward
  else stopRobot();  // Stop
}

BLYNK_WRITE(V1) {  // Left button on the app
  int value = param.asInt();
  if (value == 1) goLeft();  // Move left
  else stopRobot();  // Stop
}

BLYNK_WRITE(V2) {  // Right button on the app
  int value = param.asInt();
  if (value == 1) goRight();  // Move right
  else stopRobot();  // Stop
}

BLYNK_WRITE(V5) {
  speedCar = param.asInt();
}


void loop() {
  Blynk.run();  // Run the Blynk loop to handle communication
}
