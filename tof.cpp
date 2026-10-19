#pragma once
#include "tof.h"

void initToF(int XSHUT, int TOF_ADDR, Adafruit_VL53L0X &tof) {
  digitalWrite(XSHUT, HIGH);
  delay(10);

  if (!tof.begin(TOF_ADDR)) {
    Serial.println("Error initializing ToF");
    while (1);
  }
}

int readToF(Adafruit_VL53L0X &tof) {
  VL53L0X_RangingMeasurementData_t measure;
  
  tof.rangingTest(&measure, false); 

  return (measure.RangeStatus != 4) ? measure.RangeMilliMeter : -1;
}

void printDist(int dist){
  if(dist >= 0) Serial.printf("Distance (mm): %d|\n", dist);
  else Serial.print(" out of range ");
}

