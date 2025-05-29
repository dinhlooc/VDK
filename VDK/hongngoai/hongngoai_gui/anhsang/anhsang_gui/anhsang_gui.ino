#include <IRremote.h>

const int irPin = 3;        // Chân phát IR
const int lightSensorPin = A0;  // Chân cảm biến ánh sáng (LDR)

IRsend irsend;

void setup() {
  Serial.begin(9600);
  pinMode(irPin, OUTPUT);
}

void loop() {
  int lightValue = analogRead(lightSensorPin);
  Serial.print("Gia tri anh sang: ");
  Serial.println(lightValue);

  uint8_t command;

  // Ngưỡng ánh sáng: >500 là sáng, <=500 là tối
  if (lightValue > 500) {
    command = 0x01;
    Serial.println("Anh sang cao - Gui 0x01");
  } else {
    command = 0x00;
    Serial.println("Anh sang thap - Gui 0x00");
  }

  irsend.sendRC5(command, 12);  // Gửi tín hiệu IR (RC5)
  delay(1000);
}
