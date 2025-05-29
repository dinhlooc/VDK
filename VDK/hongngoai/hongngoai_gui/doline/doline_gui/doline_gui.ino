#include <IRremote.h>

const int irPin = 3;         // Chân phát IR
const int lineSensorPin = 8; // Chân cảm biến dò line (digital)

IRsend irsend;

void setup() {
  Serial.begin(9600);
  pinMode(irPin, OUTPUT);
  pinMode(lineSensorPin, INPUT);
}

void loop() {
  int sensorValue = digitalRead(lineSensorPin);
  Serial.print("Gia tri cam bien: ");
  Serial.println(sensorValue);

  uint8_t command;

  if (sensorValue == LOW) {
    command = 0x00; // Line đen
    Serial.println("Phat hien line den - Gui 0x00");
  } else {
    command = 0x01; // Line trắng
    Serial.println("Phat hien line trang - Gui 0x01");
  }

  irsend.sendRC5(command, 12);
  delay(1000);
}
