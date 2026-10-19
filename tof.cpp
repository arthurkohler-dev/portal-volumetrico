#pragma once
#include "tof.h"

void initToF(int XSHUT, int TOF_ADDR, Adafruit_VL53L0X &tof) {
  digitalWrite(XSHUT, HIGH);
  delay(10);

  if (!tof.begin(TOF_ADDR)) {
    Serial.println(F("Error initializing ToF"));
    while (1);
  }
}

void readToF(Adafruit_VL53L0X &tof) {
  VL53L0X_RangingMeasurementData_t measure;
  
  tof.rangingTest(&measure, false); 

  if (measure.RangeStatus != 4) {  
    Serial.print("Distance (mm): "); 
    Serial.printf("%d", measure.RangeMilliMeter);
    Serial.print("|");
  } else {
    Serial.print(" out of range ");
  }
    
  delay(100);
}
