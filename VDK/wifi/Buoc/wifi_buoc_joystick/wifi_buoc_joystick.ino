#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Stepper.h>
#include <String.h>
#include "wifi_buoc_joystick.h"

#define TENWIFI "chithanh"
#define PASSWIFI "mottoichin"

const int stepperRevolution = 2048;
Stepper myStepper(stepperRevolution, D1, D3, D2, D4);

ESP8266WebServer server(80);

int joyButtonPin = D5;  // chân nút nhấn của joystick
int analogPin = A0;     // chân analog A0 dùng cho trục X joystick

int receivedValue = 0;
bool isStepped = false;
int offset = 0;

int degreeToSteps(int degree) {
  if (degree == 0) return 0;
  return stepperRevolution / (360 / degree);
}

void connection() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void handleReadSerial() {
  int xValue = analogRead(analogPin);
  int buttonState = digitalRead(joyButtonPin);
  String result = "Joystick X: " + String(xValue);
  result += " | Nút nhấn: " + String(buttonState == LOW ? "Được nhấn" : "Chưa nhấn");
  server.send(200, "text/plain", result);
}

void handleSendValue() {
  if (server.hasArg("degrees")) {
    String recString = server.arg("degrees");
    bool isNeg = (recString[0] == '-') ? true : false;
    if (isNeg) {
      recString[0] = '0';
      receivedValue = -1 * recString.toInt();
    } else {
      receivedValue = recString.toInt();
    }
  }
}

void setup() {
  myStepper.setSpeed(14);
  Serial.begin(9600);

  pinMode(joyButtonPin, INPUT_PULLUP); // đọc nút nhấn với trở kéo lên

  WiFi.begin(TENWIFI, PASSWIFI);
  Serial.print("\nĐang kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nKết nối thành công. IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", connection);
  server.on("/readSerial", handleReadSerial);
  server.on("/sendValue", handleSendValue);
  server.begin();
  Serial.println("Server đã khởi động.");
}

void loop() {
  server.handleClient();
  unsigned long currentMillis = millis();

  if (receivedValue != 0 && currentMillis % 500 <= 15 && !isStepped) {
    int degree = (receivedValue < 0) ? receivedValue - offset : receivedValue + offset;
    Serial.println(degree);
    myStepper.step(degreeToSteps(degree));
    isStepped = true;
  } else {
    isStepped = false;
  }
}
