#include <IRremote.h>
#include <Servo.h>

Servo myServo;

const int RECV_PIN = 2;   // Chân nhận tín hiệu IR
const int SERVO_PIN = 9;  // Chân servo

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  irrecv.enableIRIn();  // Bắt đầu nhận IR
  Serial.println("IR Receiver & Servo Ready.");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Received IR code: 0x");
    Serial.println(results.value, HEX);

    if (results.value == 0x01) {
      Serial.println("Servo quay 90 độ");
      myServo.write(90);
      delay(500);
      myServo.write(0);
    } else if (results.value == 0x00) {
      Serial.println("Servo quay 180 độ");
      myServo.write(180);
      delay(500);
      myServo.write(0);
    }

      irrecv.resume();// Chuẩn bị nhận tín hiệu tiếp theo
  }
  
}
