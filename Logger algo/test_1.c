#include "test_1.h"

// todo remove: just added for compilation
void main(){}

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

/*
* Controlling the parameters
*/
// The first test to do is with P = 0, since spring is the P
float P = 0, D = 2;
// Used in filters
float safezone = 0.07;
void setPD(float p, float d){
	P = p;
	D = d;
}
void setSafezone(float val){
	safezone = val;
}

/*************************************************************/

float getAcc(float pos, float dt, CONTROL_F cf, FILTER_F ff){
	float control = (*cf)(pos, dt);
	float filter = (*ff)(pos, control);
	return clamp(filter, -1, 1);
}

/*
* Filter algorithms
*/

float filtNone(float pos, float f){
	return f;
}
float filtAggressive(float pos, float f){
	// At all costs, keep within safe zone
	return absf(pos) > safezone ? -sgn(pos) : f;
}
float filtLerp(float pos, float f){
	// At *some* costs, keep within safe zone :D
	// Basically, linearly interpolate from control value to max force when approaching limit

	if(absf(pos) > safezone){
		float p = (absf(pos) - safezone) / (movementRange - safezone);
		return lerp(f, -sgn(pos), p);
	}
	
	return f;
}


/*
* Control algorithms
*/

float lastpos;
float controlPD(float p, float dt){
	float v = (p - lastpos) * dt;
	lastpos = p;
	return -p * P + v * D;
}