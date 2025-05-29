#include <IRremote.h>
#include <Stepper.h>

const int stepPerRevolution = 2048;
Stepper myStepper(stepPerRevolution, 8, 10, 9, 11);

const int irPin = 2;
IRrecv irrecv(irPin);
decode_results results;

void setup() {
  myStepper.setSpeed(14);
  Serial.begin(9600);
  irrecv.enableIRIn();  // Bắt đầu nhận tín hiệu IR
}

int degreeToSteps(int degree) {
  if (degree == 0) return 0;
  return stepPerRevolution / (360 / degree);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Received: ");
    Serial.println(results.value, HEX);

    if (results.value == 0x1) {
      myStepper.step(degreeToSteps(60));
    } else if (results.value == 0x0) {
      myStepper.step(-degreeToSteps(120));
    }

    irrecv.resume();  // Chuẩn bị nhận tín hiệu tiếp theo
    delay(500);
  }
}
