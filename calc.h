#pragma once
#include <stdint.h>
#include <math.h>

enum CalcStatus : uint8_t {
  CALC_OK = 0,
  CALC_ERR_FEW_SAMPLES,
  CALC_ERR_BUFFER_FULL,
  CALC_ERR_ZERO_DS,
  CALC_ERR_LOW_CONF,
  CALC_ERR_SENSOR
};

struct Sample {
  uint32_t step;
  uint16_t left;
  uint16_t right;
  uint16_t top;
};

struct Calib {
  float mm_per_step;
  float D;
  float base_left;
  float base_right;
  float base_top;
  float ds_min;
};


struct BoxResult {
  float length;
  float width;
  float height;
  float theta;
  uint8_t status;
};

BoxResult calcBox(const Sample* samples, int n, const Calib &calib);
