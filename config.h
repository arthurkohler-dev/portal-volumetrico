#pragma once
//motor
#define DIR             12
#define STEP            14
#define STEPS_PER_REV   200

//ToF sensors
#define I2C_SDA         21
#define I2C_SCL         22

#define XSHUT_LEFT      26
#define XSHUT_TOP       25
#define XSHUT_RIGHT     32

#define TOF_LEFT_ADDR   0x30
#define TOF_TOP_ADDR    0x31
#define TOF_RIGHT_ADDR  0x32

#define MARGEM_MM       20

//Samples
#define K_STEPS         10
#define MM_PER_STEP     0.5f
#define COMPR_MAX_MM    400
#define MAX_SAMPLES     ((int)(COMPR_MAX_MM / (K_STEPS * MM_PER_STEP) * 1.3f))
#define N_FIM           5
#define MAX_FALHAS      25
