#include <IRremote.h>

const int irPin = 3;        // Chân phát IR
const int PINO_SGAS = A0;   // Chân cảm biến khí ga

IRsend irsend;              // Đối tượng phát IR

void setup() {
  Serial.begin(9600);
  pinMode(irPin, OUTPUT);  // Chân phát IR
}

void loop() {
  int val = analogRead(PINO_SGAS);
  Serial.print("Gia tri analog: ");
  Serial.println(val);

  uint8_t command = (val > 300) ? 0x1 : 0x0;

  Serial.print("Command gui: 0x");
  Serial.println(command, HEX);

  // Phát tín hiệu theo chuẩn RC5 (có thể thay bằng NEC nếu bạn muốn)
  irsend.sendRC5(command, 12);  // 12 bit cho RC5

  delay(1000);
}
