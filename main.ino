#include "tof.h"
#include "motor.h"

Motor* motor;
Adafruit_VL53L0X tof_left = Adafruit_VL53L0X();
Adafruit_VL53L0X tof_top = Adafruit_VL53L0X();
Adafruit_VL53L0X tof_right = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  while (! Serial) {
    delay(1);
  }

  Wire.begin(SDA, SCL);

  pinMode(XSHUT_LEFT, OUTPUT);
  pinMode(XSHUT_TOP, OUTPUT);
  pinMode(XSHUT_RIGHT, OUTPUT);

  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_TOP, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);


  initToF(XSHUT_LEFT, TOF_LEFT_ADDR, tof_left);
  initToF(XSHUT_TOP, TOF_TOP_ADDR, tof_top);
  initToF(XSHUT_RIGHT, TOF_RIGHT_ADDR, tof_right);

  motor = initMotor();

}

void loop() {
  revolMotor(motor);
  readToF(tof_left);
  readToF(tof_top);
  readToF(tof_right);
  Serial.printf("\n");
}
