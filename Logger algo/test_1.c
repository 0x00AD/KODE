#include "test_1.h"
#include <stdio.h>

// todo remove: just added for compilation
void main(){
	printf("Hello World!\n");
	options opt = createOptions(1, 1, 1, 0, 0);
	printf("%f\n", getAcc(0.1, 0.02, opt));
}

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

int control_count = 1;
float (*controls[])(float, float, options) = {controlPD};
int filter_count = 3;
float (*filters[])(float, float, options) = {filtNone, filtAggressive, filtLerp};

options createOptions(float P, float D, float safezone, int control_ind, int filter_ind){
	return (options){P, D, safezone, controls[control_ind % control_count], filters[filter_ind / filter_count]};
}

/*************************************************************/

float getAcc(float pos, float dt, options opt){
	float control = (*opt.cf)(pos, dt, opt);
	float filter = (*opt.ff)(pos, control, opt);
	return clamp(filter, -1, 1);
}

/*
* Filter algorithms
*/

float filtNone(float pos, float f, options opt){
	return f;
}
float filtAggressive(float pos, float f, options opt){
	// At all costs, keep within safe zone
	return absf(pos) > opt.safezone ? -sgn(pos) : f;
}
float filtLerp(float pos, float f, options opt){
	// At *some* costs, keep within safe zone :D
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

float lastpos;
float controlPD(float p, float dt, options opt){
	float v = (p - lastpos) * dt;
	lastpos = p;
	return -p * opt.P + v * opt.D;
}