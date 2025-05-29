#include <IRremote.h>

const int irPin = 3;        // Chân phát IR
const int trigPin = 7;      // Chân TRIG của HC-SR04
const int echoPin = 6;      // Chân ECHO của HC-SR04

IRsend irsend;

void setup() {
  Serial.begin(9600);
  pinMode(irPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  long distance = readDistanceCM();
  Serial.print("Khoang cach (cm): ");
  Serial.println(distance);

  uint8_t command = (distance < 20) ? 0x00 : 0x01;

  Serial.print("Command gui: 0x");
  Serial.println(command, HEX);

  irsend.sendRC5(command, 12);

  delay(1000);
}
