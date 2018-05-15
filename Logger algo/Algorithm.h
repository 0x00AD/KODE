
#define movementRange 0.07
#define dampingFactor 0.98
#define levelP 1
#define levelD 1
struct opts;

/*
* The filter functions are of form
* float filtFunc(float pos, float input_val)
*
* The control functions are of form
* float controlFunc(float pos, float acc_z, float dt)
*/
typedef float (*FILTER_F)(float, float, struct opts);
typedef float (*CONTROL_F)(float, float, float, struct opts);

typedef struct opts {
	float P;
	float D;
	float safezone;
	CONTROL_F cf;
	FILTER_F ff;
} options;

typedef struct p {
	float A;
	float B;
} pair;

/**
* Accessibility function to choose the functions with simple indexing
* Current ranges: control: 2, filter: 3
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
*			or with voltage mode, just scale so that 1 is max voltage
*/
float getAcc(float pos, float acc_z, float dt, options opt);
pair getAccLevel(float pos, float acc_z, float dt, options opt);


/*
* Filter algorithms
*/
float filtNone(float pos, float f, options opt);
float filtAggressive(float pos, float f, options opt);
float filtLerp(float pos, float f, options opt);

/*
* Control algorithms
*/
float controlPD(float pos, float acc_z, float dt, options opt);
float controlMagic(float pos, float acc_z, float dt, options opt);
float controlMagicInBed(float pos, float acc_z, float dt, options opt);

