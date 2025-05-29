#include <SoftwareSerial.h>
#include <Stepper.h>

SoftwareSerial BTSerial(10, 11); // (Rx, Tx)
const int stepPerRevolution = 2048;
Stepper myStepper(stepPerRevolution, 6, 8, 7, 9);

unsigned long lastStepTime = 0;
const unsigned long stepInterval = 300; // Adjust as necessary
int stepAmount = 0;

void setup() {
  myStepper.setSpeed(16);
  Serial.begin(9600);
  BTSerial.begin(38400);
}

int degreeToSteps(int degree) {
  if (degree == 0) return 0;
  return stepPerRevolution / (360 / degree);
}

void loop() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    Serial.println(data);

    switch (data) {
      case '0': // Forward
        stepAmount = degreeToSteps(60);
        break;
      case '1': // Backward
        stepAmount = -degreeToSteps(60);
        break;
      case '2': // Right
        stepAmount = degreeToSteps(180);
        break;
      case '3': // Left
        stepAmount = -degreeToSteps(180);
        break;
      case '4': // Press button
        stepAmount = degreeToSteps(90); // hành động khi nhấn nút
        break;
      default:
        stepAmount = 0;
        break;
    }
  }

  if (stepAmount != 0 && millis() - lastStepTime >= stepInterval) {
    myStepper.step(stepAmount);
    lastStepTime = millis();
    stepAmount = 0;
  }
}
