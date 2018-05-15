#include "Algorithm.h"

// Some useful math functions (no imports needed)
float absf(float f){
    return f < 0 ? -f : f;
}
float lerp(float val0, float val1, float p){
    return val0 + p * (val1 - val0);
}
float map(float val, float from0, float from1, float to0, float to1){
    return (val - from0) / (from1 - from0) * (to1 - to0) + to0;
}
float clamp(float val, float min, float max){
    return val < min ? min : val > max ? max : val;
}
int sgn(float val) {
   return (0 < val) - (val < 0);
}

int control_count = 3;
float (*controls[])(float, float, float, options) = {controlPD, controlMagic, controlMagicInBed};
int filter_count = 3;
float (*filters[])(float, float, options) = {filtNone, filtAggressive, filtLerp};

options createOptions(float P, float D, float safezone, int control_ind, int filter_ind){
    return (options){P, D, safezone, controls[control_ind % control_count], filters[filter_ind % filter_count]};
}

/*************************************************************/

float getAcc(float pos, float acc_z, float dt, options opt){
    float control = (*opt.cf)(pos, acc_z, dt, opt);
    float filter = (*opt.ff)(pos, control, opt);
    return clamp(filter, -1, 1);
}

// NO GUARANTEES!!!
float lastDiff = 0;
float getAccLevel(float posA, float posB, float acc_z, float dt, options opt){
	// control function
    float controlA = (*opt.cf)(posA, acc_z, dt, opt);
    float controlB = (*opt.cf)(posB, acc_z, dt, opt);

	// differential PID
	float diff = posB - posA;
	float diffv = (diff - lastDiff) / dt;
	
	float diffPID = -(levelP * diff + levelD * diffV);
	
	float cA = lerp(controlA, diffPID, MIN(1, abs(diffPID)));
	float cB = lerp(controlB, -diffPID, MIN(1, abs(diffPID)));
	
	lastDiff = diff;

	// filter function
    float filterA = (*opt.ff)(posA, cA, opt);
    float filterB = (*opt.ff)(posB, cB, opt);
    return (pair){clamp(filterA, -1, 1), clamp(filterB, -1, 1)};
}

/*
* Filter algorithms
*/

float filtNone(float pos, float f, options opt){
    return absf(pos) > movementRange - 0.01 ? (pos < 0 ? MAX(0, f) : MIN(0, f)) : f;
}
float filtAggressive(float pos, float f, options opt){
    // At all costs, keep within safe zone
    return absf(pos) > opt.safezone ? -sgn(pos) : f;
}
float filtLerp(float pos, float f, options opt){
    // At *some* costs, keep within safe zone :smile:
    // Basically, linearly interpolate from control value to max force when approaching limit

    if(absf(pos) > opt.safezone){
        float p = (absf(pos) - opt.safezone) / (movementRange - opt.safezone);
        return lerp(f, -sgn(pos), p);
    }

    return f;
}

/*
* Control algorithms
*/

float lastpos = 0;
float controlPD(float pos, float acc_z, float dt, options opt){
    float v = (pos - lastpos) / dt;
    lastpos = pos;
    return -(pos * opt.P + v * opt.D);
}

float targetpos = 0;
float targetvel = 0;
float controlMagic(float pos, float acc_z, float dt, options opt){
	targetpos *= dampingFactor
    targetvel *= dampingFactor;
	
    targetpos += (targetvel + 0.5 * acc_z * dt) * dt;
    targetpos = clamp(targetpos, -movementRange, movementRange);
    targetvel += acc_z * dt;
    float v = (pos - lastpos) / dt;
    lastpos = pos;
    return -((pos - targetpos) * opt.P + (v - targetvel) * opt.D);
}

float bedVel = 0;
float controlMagicInBed(float pos, float acc_z, float dt, options opt){
	bedVel *= dampingFactor;
    bedVel += acc_z * dt;
    return -bedVel * opt.D;
}