#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "lightSensor_Servo.h"

Servo myservo;
ESP8266WebServer server(80);

#define lightSensor A0
#define servoPin D1

#define TENWIFI "chithanh"
#define PASSWIFI "mottoichin"

void connection() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void setup() {
  myservo.attach(servoPin, 500,2400);
  Serial.begin(9600);

  WiFi.begin(TENWIFI, PASSWIFI);
  Serial.print("\n Đang thử kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay (500); 
    Serial.print(".");
  }
  Serial.print("\n Kết nối thanh công. IP address: ");
  Serial.println(WiFi.localIP());
  server.begin(); // Khởi động server.
  Serial.print("Server đã khởi động.");

  server.on("/", connection);
  server.on("/readSerial", handleReadSerial);
  server.on("/sendValue", handleSendValue);
  server.begin();
}

void handleReadSerial() {
  // Gửi giá trị cảm biến về client
  float light = analogRead(lightSensor);
  server.send(200, "text/plain", "Giá trị cảm biến ánh sáng: " + String(light));
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
  // Xử lý các yêu cầu từ client
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
    //myStepper.step(degreeToSteps(degree));
    myservo.write(degree);
    delay(800);
    myservo.write(0);
    delay(800);
    isSpined = true;
  } else {
    isSpined = false;
  }
}
