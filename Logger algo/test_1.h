
#define movementRange 0.12
struct options;
/*
* The filter functions are of form
* float filtFunc(float pos, float input_val)
*
* The control functions are of form
* float controlFunc(float pos, float dt)
*/
typedef float (*FILTER_F)(float, float, struct options);
typedef float (*CONTROL_F)(float, float, struct options);

typedef struct options {
	float P;
	float D;
	float safezone;
	CONTROL_F cf;
	FILTER_F ff;
} options;

/**
* Accessibility function to choose the functions with simple indexing
* Current ranges: control: 1, filter: 3
* Too big indices are fixed with modulus
*/
options createOptions(float P, float D, float safezone, int control_ind, int filter_ind);

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
float getAcc(float pos, float dt, options opt);



/*
* Filter algorithms
*/
float filtNone(float pos, float f, options opt);
float filtAggressive(float pos, float f, options opt);
float filtLerp(float pos, float f, options opt);

/*
* Control algorithms
*/
float controlPD(float p, float dt, options opt);

