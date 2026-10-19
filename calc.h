#pragma once
#include <stdint.h>
#include <math.h>

enum CalcStatus : uint8_t {
  CALC_OK = 0,
  CALC_ERR_FEW_SAMPLES,
  CALC_ERR_BUFFER_FULL,
  CALC_ERR_ZERO_DS,
  CALC_ERR_LOW_CONF,
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

struct Faces{
  float A;
  float B;
  float theta;
  uint8_t status;
};

struct BoxResult {
  float length;   
  float width;    
  float height;
  float theta;    
  uint8_t status; 
};

bool rising(const Sample* samples, int i);
Faces fitFaces (const Sample* samples, int n, const Calib &calib);
float calcHeight(const Sample* samples, int n, const Calib &calib);
float calcLength(Faces f);
float calcWidth (Faces f);
float calcTheta (Faces f);
BoxResult calcBox(const Sample* samples, int n, const Calib &calib);
