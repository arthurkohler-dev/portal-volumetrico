#pragma once
#include "calc.h"
#include "tof.h"
#include "motor.h"
#include "config.h"


bool measureBaseline(Calib &c);
BoxResult acquireBox(Motor* motor, const Calib &calib);
