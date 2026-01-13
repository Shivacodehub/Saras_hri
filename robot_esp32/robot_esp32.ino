#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>

WebServer server(80);
Servo neck;

// ------------------ PINS ------------------
#define TRIG 5
#define ECHO 18

// MOTOR PINS (EDIT AS PER DRIVER)
#define M1 25
#define M2 26
#define M3 27
#define M4 14
// ------------------------------------------

void setup() {
  WiFi.softAP("RobotESP32");
  server.on("/", HTTP_POST, handleCommand);
  server.begin();

  neck.attach(13);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
}

long getDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) * 0.034 / 2;
}

void stopMotors() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, LOW);
}

void moveForward() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}

void moveBackward() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
}

void turnLeft() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}

void turnRight() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
}

void handleCommand() {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, server.arg("plain"));

  String intent = doc["intent"];
  String direction = doc["direction"];
  int distance = doc["distance_cm"];
  int neckAngle = doc["neck_angle"];

  // --- NECK CONTROL ---
  if (neckAngle >= 0 && neckAngle <= 180) {
    neck.write(neckAngle);
  }

  // --- MOVEMENT ---
  if (intent == "MOVE") {
    long travelled = 0;

    while (travelled < distance) {
      if (getDistance() < 25) { // obstacle
        stopMotors();
        break;
      }

      if (direction == "FORWARD") moveForward();
      else if (direction == "BACKWARD") moveBackward();
      else if (direction == "LEFT") turnLeft();
      else if (direction == "RIGHT") turnRight();

      delay(100);
      travelled += 5;
    }
    stopMotors();
  }

  // --- ACK ---
  StaticJsonDocument<100> res;
  res["ack"] = "DONE";

  String out;
  serializeJson(res, out);
  server.send(200, "application/json", out);
}

void loop() {
  server.handleClient();
}
