#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // Rx, Tx

int ENA = 9;
int IN1 = 7;
int IN2 = 8;

int currentDirection = -1; // -1: chưa có hướng, 0: nghịch, 1: thuận

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
  BTSerial.begin(38400);
}

void setMotor(int direction, int speed) {
  if (direction == -1) {  // dừng động cơ
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
    currentDirection = -1;
    return;
  }

  // Nếu đổi hướng thì dừng trước
  if (currentDirection != -1 && currentDirection != direction) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
    delay(300);
  }

  currentDirection = direction;

  if (direction == 1) { // Thuận
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (direction == 0) { // Nghịch
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  analogWrite(ENA, speed);
}

void loop() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    Serial.println(data);

    if (data == '1') {
      setMotor(1, 38);    // Thuận, tốc 38
    } else if (data == '0') {
      setMotor(0, 50);    // Nghịch, tốc 50
    } else if (data == '2') {
      setMotor(1, 100);   // Thuận, tốc 100
    } else if (data == '3') {
      setMotor(0, 50);    // Nghịch, tốc 50
    } else if (data == '4') {
      setMotor(-1, 0);    // Dừng động cơ
    }
  }
}
