#pragma once
#include <Arduino.h>
#include <Wire.h> //lib used for I2C comunnication
#include "config.h"
#include "Adafruit_VL53L0X.h"

void initToF(int XSHUT, int TOF_ADDR, Adafruit_VL53L0X &tof);
void readToF(Adafruit_VL53L0X &tof);
