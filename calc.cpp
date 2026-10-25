#include "calc.h"

struct Faces{
  float A;
  float B;
  float theta;
  uint8_t status;
};

static bool rising(const Sample* samples, int i) { return samples[i].left < samples[i+1].left;}
static float dSteps (const Sample* s, int a, int b) { return float(s[b].step - s[a].step); }
static float dLeft  (const Sample* s, int a, int b) { return float(s[b].left - s[a].left); }
static float calcExtent(const Sample* s, int n, const Calib &calib){
  int i;
  float min_left = s[0].left, min_right = s[0].right;
  for(i = 1; i < n; i++){
    min_left  = min_left  < s[i].left  ? min_left  : s[i].left;
    min_right = min_right < s[i].right ? min_right : s[i].right;
  }
  return calib.D - (min_left + min_right);
}

static Faces fitFaces (const Sample* samples, int n, const Calib &calib){
  Faces faces = {};
  if (n < 3) { faces.status = CALC_ERR_FEW_SAMPLES; return faces;}
  
  float delta_sA, delta_sB, delta_s, delta_d, cross;
  int switch_i, i = 1;
  bool slope = rising(samples, 0);
  
  while (i < n-1 && rising(samples, i) == slope) i++;
  switch_i = i;

  delta_sA = dSteps(samples, 0, i);
  slope = !slope;  
  while (i < n-1 && rising(samples, i) == slope) i++;
  delta_sB = dSteps(samples, switch_i, i);
  
  if (delta_sA > delta_sB){
    delta_s = delta_sA;
    delta_d = dLeft(samples, 0, switch_i);
  }else{
    delta_s = delta_sB;
    delta_d = dLeft(samples, switch_i, i);
  }

  delta_s = delta_s * calib.mm_per_step;
  delta_d = fabsf(delta_d);
  
  if (delta_s < calib.ds_min) { faces.status = CALC_ERR_ZERO_DS; return faces;}
  
  cross = calcExtent(samples, n, calib);
  faces.A = sqrtf(delta_s*delta_s + delta_d*delta_d);
  faces.B = (cross - delta_d)*faces.A/delta_s;
  
  if (faces.B <= 0) { faces.status = CALC_ERR_LOW_CONF; return faces; }
  
  faces.theta = atan2f(delta_d, delta_s);
  faces.status = 0;
  
  return faces;
}

static float calcHeight(const Sample* samples, int n, const Calib &calib){
  int i;
  float min_height = samples[0].top;
  for (i = 1; i < n; i++)
       min_height = (min_height < samples[i].top) ? min_height : samples[i].top;
  return (calib.base_top - min_height);
}

static float calcLength(Faces f){ return fmaxf(f.A, f.B);}
static float calcWidth (Faces f){ return fminf(f.A, f.B);}
static float calcTheta (Faces f){ return f.theta;}

BoxResult calcBox(const Sample* samples, int n, const Calib &calib){
  Faces faces = fitFaces(samples, n, calib);
  BoxResult box = {};
  if (faces.status != CALC_OK) { box.status = faces.status; return box; }

  float length, width, height, theta;
  
  length = calcLength(faces);
  width  = calcWidth (faces);
  height = calcHeight(samples, n, calib);
  theta  = calcTheta (faces);


  box.height = height;
  box.length = length;
  box.width  = width;
  box.theta  = theta;
  box.status = faces.status;

  return box;
}
