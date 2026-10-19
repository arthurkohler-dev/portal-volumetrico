#include "calc.h"

bool rising(const Sample* samples, int i) { return samples[i].left < samples[i+1].left;}

Faces fitFaces (const Sample* samples, int n, const Calib &calib){
  Faces faces = {};
  if (n < 3) { faces.status = CALC_ERR_FEW_SAMPLES; return faces;}
  float delta_sA, delta_sB, delta_s, delta_d, min_left = samples[0].left, min_right = samples[0].right;
  int switch_i;
  float cross;

  int i = 1;
  bool slope = (samples[0].left < samples[1].left);
  while (i < n-1 && (samples[i].left < samples[i+1].left) == slope) i++;

  switch_i = i;
  delta_sA = samples[i].step - samples[0].step;
  slope = !slope;  
  while (i < n-1 && (samples[i].left <= samples[i+1].left) == slope) i++;
  delta_sB = samples[i].step - samples[switch_i].step;
  
  if (delta_sA > delta_sB){
    delta_s = delta_sA;
    delta_d = samples[switch_i].left - samples[0].left;
  }else{
    delta_s = delta_sB;
    delta_d = samples[i].left - samples[switch_i].left;
  }

  for(int k = 1; k < n; k++){
    min_left = min_left < samples[k].left ? min_left : samples[k].left;
    min_right = min_right < samples[k].right ? min_right : samples[k].right;
  }

  delta_s = delta_s * calib.mm_per_step;
  delta_d = fabsf(delta_d);
  if (delta_s < calib.ds_min) { faces.status = CALC_ERR_ZERO_DS; return faces;}
  cross = calib.D - (min_left + min_right);
  faces.A = sqrtf(delta_s*delta_s + delta_d*delta_d);
  faces.B = (cross - delta_d)*faces.A/delta_s;
  faces.theta = atan2f(delta_d, delta_s);
  faces.status = 0;

  return faces;
}

float calcHeight(const Sample* samples, int n, const Calib &calib){
  int i;
  float min_height = samples[0].top;
  for (i = 1; i < n; i++)
       min_height = (min_height < samples[i].top) ? min_height : samples[i].top;
  return (calib.base_top - min_height);
}

float calcLength(Faces f){ return fmaxf(f.A, f.B);}
float calcWidth (Faces f){ return fminf(f.A, f.B);}
float calcTheta (Faces f){ return f.theta;}

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
