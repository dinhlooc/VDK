#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#include "DHT_DC.h"

ESP8266WebServer server(80);

#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define ENA D1
#define IN1 D2
#define IN2 D3

#define TENWIFI "chithanh"
#define PASSWIFI "mottoichin"

const int maxSpeed = 255;
const int minSpeed = 0;
const int step = 20;
int currentSpeed = 100;
bool DCdirection = true;  // true: forward, false: backward

// ======= HTML Giao diện ========
void connection() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

// ====== Xử lý setup ban đầu ======
void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  dht.begin();

  WiFi.begin(TENWIFI, PASSWIFI);
  Serial.print("\nĐang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi kết nối, IP: " + WiFi.localIP().toString());

  server.on("/", connection);
  server.on("/readSerial", handleReadSerial);
  server.on("/DCforward", handleDCforward);
  server.on("/DCbackward", handleDCbackward);
  server.on("/DCspeedup", handleDCspeedUp);
  server.on("/DCspeeddown", handleDCspeedDown);
  server.begin();
}

// ====== Xử lý các API gọi từ client ======
void handleReadSerial() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    server.send(200, "text/plain", "Không đọc được cảm biến!");
    return;
  }

  server.send(200, "text/plain", "Nhiệt độ: " + String(t) + " °C | Độ ẩm: " + String(h) + " %");
}

void handleDCforward() {
  DCdirection = true;
  server.send(200, "text/plain", "Đi tới");
}

void handleDCbackward() {
  DCdirection = false;
  server.send(200, "text/plain", "Đi lùi");
}

void handleDCspeedUp() {
  currentSpeed = min(currentSpeed + step, maxSpeed);
  server.send(200, "text/plain", "Tăng tốc độ: " + String(currentSpeed));
}

void handleDCspeedDown() {
  currentSpeed = max(currentSpeed - step, minSpeed);
  server.send(200, "text/plain", "Giảm tốc độ: " + String(currentSpeed));
}

// ====== Hàm chạy động cơ ======
void moveMotor() {
  analogWrite(ENA, currentSpeed);
  if (currentSpeed == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    return;
  }

  if (DCdirection) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

// ====== Vòng lặp chính ======
void loop() {
  server.handleClient();
  moveMotor(); // chỉ cập nhật motor khi có thay đổi
}
