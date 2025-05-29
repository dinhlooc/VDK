#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "TurboSensor_Servo.h" 

Servo myservo;
ESP8266WebServer server(80);

// Chân joystick và nút nhấn
int joystickXPin = A0;  // ADC chân đọc trục X joystick
int buttonPin = D5;     // Chân digital đọc nút nhấn

#define TENWIFI "chithanh"
#define PASSWIFI "mottoichin"

void connection() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Nút nhấn active LOW
  myservo.attach(D1, 500, 2400);

  Serial.begin(9600);

  WiFi.begin(TENWIFI, PASSWIFI);
  Serial.print("\n Đang thử kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }
  Serial.print("\n Kết nối thành công. IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", connection);
  server.on("/readSerial", handleReadSerial);
  server.on("/sendValue", handleSendValue);
  server.begin();
  Serial.println("Server đã khởi động.");
}

void handleReadSerial() {
  int xValue = analogRead(joystickXPin);
  int buttonState = digitalRead(buttonPin);
  String buttonText = (buttonState == LOW) ? "Pressed" : "Released";

  Serial.print("Joystick X: ");
  Serial.print(xValue);
  Serial.print(" | Button: ");
  Serial.println(buttonText);

  server.send(200, "text/plain", "Joystick X: " + String(xValue) + ", Button: " + buttonText);
}

int receivedValue = 0;
void handleSendValue() {
  if (server.hasArg("degrees")) {
    receivedValue = server.arg("degrees").toInt();
  }
}

bool isSpined = false;
int offset = 0;

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();  

  if (receivedValue != 0 && currentMillis % 500 <= 15 && !isSpined) {
    int degree = receivedValue;
    if (degree > 0) {
      degree += offset;
    } else {
      degree -= offset;
    }
    Serial.println(degree);
    myservo.write(degree);
    delay(800);
    myservo.write(0);
    delay(800);
    isSpined = true;
  } else {
    isSpined = false;
  }
}
