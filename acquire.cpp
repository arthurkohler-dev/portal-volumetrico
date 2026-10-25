#include "acquire.h"


static bool readAll(Sample &s){ 
  int id, dist[TOF_COUNT];
  for ( id = 0; id < TOF_COUNT; id++){
    dist[id] = readTof((TofId)id);
    if (dist[id] < 0) return false; 
  }
  s.left = dist[TOF_LEFT];
  s.top = dist[TOF_TOP];
  s.right = dist[TOF_RIGHT];
  return true;
}

static bool readRetry(Sample &s){
  for (int i = 0; i < MAX_FALHAS; i++) if (readAll(s)) return true;
  return false;
}

static bool present(const Sample &s, const Calib &c){
  return (
    s.left  < (c.base_left  - MARGEM_MM) ||
    s.right < (c.base_right - MARGEM_MM) ||
    s.top   < (c.base_top   - MARGEM_MM));
}

static void advance(Motor* m, int k){
  for (int i = 0; i < k; i++) stepMotor(m);
}

bool measureBaseline(Calib &c){
  int i, counter = 0;
  Sample s;
  float sum_l = 0, sum_t = 0, sum_r = 0;
  for (i = 0; i < 20; i++){
    if(readAll(s)){
      sum_l += s.left;
      sum_t += s.top;
      sum_r += s.right;
      counter++;
    }
  }
  if (counter == 0) { return false; }
  c.base_left   = sum_l/counter;
  c.base_top    = sum_t/counter;
  c.base_right  = sum_r/counter;
  return true;
}

BoxResult acquireBox(Motor* motor, const Calib &calib){
  BoxResult box = {};
  static Sample buf[MAX_SAMPLES];
  Sample s;
  do{
    int n = 0, ausents = 0;
    uint32_t step = 0;
    while (true) {
      if (!readRetry(s)) { box.status = CALC_ERR_SENSOR; return box; }
      if (present(s, calib)) break;
      advance(motor, K_STEPS);
    }
    while (true){
      if(present(s, calib)){
        ausents = 0;
        s.step = step;
        if(n == MAX_SAMPLES) {box.status = CALC_ERR_BUFFER_FULL; return box;}
        buf[n++] = s;
      }else if (++ausents >= N_FIM) break;
      step += K_STEPS;
      advance(motor, K_STEPS);
      if (!readRetry(s)) { box.status = CALC_ERR_SENSOR; return box; }
    }
    box = calcBox(buf, n, calib);
  } while (box.status == CALC_ERR_FEW_SAMPLES);
  return box;
}
