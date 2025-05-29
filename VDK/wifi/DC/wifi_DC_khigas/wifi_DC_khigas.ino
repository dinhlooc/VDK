#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "wifi_DC_khigas.h"

ESP8266WebServer server(80);

#define GAS_SENSOR_PIN A0
int gasValue = 0;

#define maxSpeed  255
#define minSpeed  0
int step = 10;
int ENA = D1;
int IN1 = D2;
int IN2 = D3;

#define TENWIFI "Tang4 Wifi"
#define PASSWIFI "vuive789@"

void connection() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  WiFi.begin(TENWIFI, PASSWIFI);
  Serial.print("\n Đang thử kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }

  Serial.print("\n Kết nối thành công. IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server đã khởi động.");

  server.on("/", connection);
  server.on("/readGas", handleReadGas);
  server.on("/DCforward", handleDCforward);
  server.on("/DCbackward", handleDCbackward);
  server.on("/DCspeedup", handleDCspeedUp);
  server.on("/DCspeeddown", handleDCspeedDown);
}

void handleReadGas() {
  gasValue = analogRead(GAS_SENSOR_PIN);
  server.send(200, "text/plain", "Giá trị khí gas: " + String(gasValue));
}

bool DCdirection = true;
void handleDCforward() {
  DCdirection = true;
  server.send(200, "text/plain", "go forward");
}

void handleDCbackward() {
  DCdirection = false;
  server.send(200, "text/plain", "go backward");
}

bool DCspeed = true;
void handleDCspeedUp() {
  DCspeed = true;
  server.send(200, "text/plain", "speedUp");
}

void handleDCspeedDown() {
  DCspeed = false;
  server.send(200, "text/plain", "speedDown");
}

void DCmove_forward() {
  analogWrite(ENA, maxSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void DCmove_backward() {
  analogWrite(ENA, maxSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void DCspeedUp() {
  for (int i = 10; i <= maxSpeed; i += step) {
    analogWrite(ENA, i);
  }
}

void DCspeedDown() {
  for (int i = maxSpeed; i >= 10; i -= step) {
    analogWrite(ENA, i);
  }
}

void loop() {
  server.handleClient();
  
  if (DCdirection) DCmove_forward();
  else DCmove_backward();
  
  // if (DCspeed) DCspeedUp();
  // else DCspeedDown();
}
