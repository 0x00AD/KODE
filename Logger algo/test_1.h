
#define movementRange 0.12

/*
* The filter functions are of form
* float filtFunc(float pos, float input_val)
*
* The control functions are of form
* float controlFunc(float pos, float dt)
*/
typedef float (*FILTER_F)(float, float);
typedef float (*CONTROL_F)(float, float);

/**
* The core control function
*
* Usage: getAcc(pos, dt, controlPD, filtNone)
*
* @input
*		float pos: the bed (end) position, normalized to meters and zeroed appropriately. Positive up.
*			It's assumed to be within [-movementRange, movementRange]
*		float dt: the time difference between the last two measurements
*		*cf: the control algorithm to be used
*		*ff: the filter algorithm to be used
* @output desired acceleration: float in the range [-1, 1]. Positive up.
*			-> if using 50-50 PWM force mode, the PWM duty cycle should be (f + 1)/2
*			or with 100% PWM force mode, the duty cycle of PWM is abs(f), the direction wire sign(f)
*/
float getAcc(float pos, float dt, CONTROL_F cf, FILTER_F ff);


/*
* These should be controllable externally for tweaking & testing:
*/
void setPD(float p, float d);
void setSafezone(float val);

/*
* Filter algorithms
*/
float filtNone(float pos, float f);
float filtAggressive(float pos, float f);
float filtLerp(float pos, float f);

/*
* Control algorithms
*/
float controlPD(float p, float dt);

