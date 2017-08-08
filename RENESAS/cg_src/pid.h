#ifndef PID_H_
#define PID_H_


#define AUTOMATIC 1
#define MANUAL 0
#define DIRECT 0
#define REVERSE 1

#define true  1
#define false 0

struct PID
{
	
	double dispKp;				// * we'll hold on to the tuning parameters in user-entered 
	double dispKi;				//   format for display purposes
	double dispKd;				//
    
	double kp;                  // * (P)roportional Tuning Parameter
    double ki;                  // * (I)ntegral Tuning Parameter
    double kd;                  // * (D)erivative Tuning Parameter

	int controllerDirection;

    double *myInput;              // * Pointers to the Input, Output, and Setpoint variables
    double *myOutput;             //   This creates a hard link between the variables and the 
    double *mySetpoint;           //   PID, freeing the user from having to constantly tell us
                                  //   what these values are.  with pointers we'll just know.
			  
	unsigned long lastTime;
	double ITerm, lastInput;

	unsigned long SampleTime;
	double outMin, outMax;
	int inAuto;
};

void xPID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd, int ControllerDirection);
int  xCompute(double * Input);
void xSetTunings(double Kp, double Ki, double Kd);
void xSetSampleTime(int NewSampleTime);
void xSetOutputLimits(double Min, double Max);
void xSetMode(int Mode);
void xInitialize(void);
void xSetControllerDirection(int Direction);

void yPID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd, int ControllerDirection);
int  yCompute(double * Input);
void ySetTunings(double Kp, double Ki, double Kd);
void ySetSampleTime(int NewSampleTime);
void ySetOutputLimits(double Min, double Max);
void ySetMode(int Mode);
void yInitialize(void);
void ySetControllerDirection(int Direction);

#endif
