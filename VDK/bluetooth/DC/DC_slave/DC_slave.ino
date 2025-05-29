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
  // Kiểm tra nếu đổi hướng thì delay
  if (currentDirection != -1 && currentDirection != direction) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
    delay(300); // Delay khi đổi chiều
  }

  currentDirection = direction;

  if (direction == 1) { // Thuận
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else { // Nghịch
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
      setMotor(1, 38); // Thuận, tốc 100
    } else if (data == '0') {
      setMotor(0, 50); // Nghịch, tốc 50
    }
  }
}
