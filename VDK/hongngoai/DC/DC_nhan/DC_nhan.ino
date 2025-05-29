#include <IRremote.h>

// Pin hồng ngoại
const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Chân L298N
const int in1 = 7;
const int in2 = 8;
const int enA = 9;

// Trạng thái chiều quay hiện tại
int currentDirection = 0; // 0: dừng, 1: tiến, -1: lùi

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
}

void stop_DC() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long irCode = results.value;

    Serial.print("Mã nhận được: 0x");
    Serial.println(irCode, HEX);

    if (irCode == 0x00 || irCode == 0x02) {  // Quay thuận (tiến)
      if (currentDirection != 1) {
        stop_DC();
        delay(500);
        currentDirection = 1;
      }
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(enA, 100);  // tốc độ 100
    }
    else if (irCode == 0x01 || irCode == 0x03) { // Quay nghịch (lùi)
      if (currentDirection != -1) {
        stop_DC();
        delay(500);
        currentDirection = -1;
      }
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(enA, 50);  // tốc độ 50
    }
    else if (irCode == 0x04) { // Dừng lại
      stop_DC();
      currentDirection = 0;
    }

    irrecv.resume(); // Sẵn sàng nhận mã tiếp theo
  }
}
