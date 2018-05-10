// todo remove: just added for compilation
void main(){}

#define movementRange 0.12

/**
* The filter functions are of form
* float filtFunc(float pos, float input_val)
*/
typedef float (*FILTER_F)(float, float);
/**
* The control functions are of form
* float controlFunc(float pos, float dt)
*/
typedef float (*CONTROL_F)(float, float);

// These should be controllable externally for tweaking & testing

// The first test to do is with P = 0, since spring is the P
float P = 0, D = 2;
// Used in algB
float safezone = 0.07;

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

/**
* The core control function
*
* Usage: algNone(pos, dt, controlPD, filtNone)
*
* @input
*	float pos: the bed (end) position, normalized to meters and zeroed appropriately. Positive up.
*		It's assumed to be within [-movementRange, movementRange]
*	float dt: the time difference between the last two measurements
*	*cf: the control algorithm to be used
*	*ff: the filter algorithm to be used
* @output desired acceleration as a float in the range [-1, 1]: positive up, negative down
*		-> if using 50-50 PWM force mode, the PWM duty cycle should be (f + 1)/2
*		or with 100% PWM force mode, the duty cycle of PWM is abs(f), the direction wire sign(f)
*/
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