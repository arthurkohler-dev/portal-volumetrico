#include "tof.h"
#include "motor.h"
#include "calc.h"


Motor* motor;
void setup() {
  Serial.begin(115200);

  while (! Serial) {
    delay(1);
  }
  initAllTof();
  motor = initMotor();
}

void loop() {
  revolMotor(motor);
  printDist(readTof(TOF_LEFT ));
  printDist(readTof(TOF_TOP  ));
  printDist(readTof(TOF_RIGHT));
  Serial.printf("\n");
}
