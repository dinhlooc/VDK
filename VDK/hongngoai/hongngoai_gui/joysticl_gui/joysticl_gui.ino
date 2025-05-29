#include <IRremote.h>

int Joystick_x = A0;
int Joystick_y = A1;
int Joystick_sw = 2;

IRsend irsend;  // KHÔNG truyền tham số chân

void setup() {
  Serial.begin(9600);
  pinMode(Joystick_sw, INPUT_PULLUP);
  // Không gọi irsend.begin()
}

void loop() {
  int x = analogRead(Joystick_x);
  int y = analogRead(Joystick_y);
  int sw = digitalRead(Joystick_sw);

  Serial.print("x = ");
  Serial.print(x);
  Serial.print("\t y = ");
  Serial.print(y);
  Serial.print("\t sw = ");
  Serial.println(sw);

  uint16_t tData = 0xFFFF;  // Không gửi nếu bằng 0xFFFF

  if (sw == LOW) {
    tData = 0x04;
  } else if (x > 1000) {
    tData = 0x00;
  } else if (x < 100) {
    tData = 0x01;
  } else if (y < 100) {
    tData = 0x02;
  } else if (y > 1000) {
    tData = 0x03;
  }

  
   irsend.sendRC5(tData, 16);
   delay(500);
  
}
