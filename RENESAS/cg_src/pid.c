#include "stdio.h"
#include "stdlib.h"
#include "pid.h"
#include "r_cg_macrodriver.h"
#include "r_cg_sci.h"
#include "r_cg_cmt.h"

struct PID x_pid, y_pid;

/*Constructor (...)*********************************************************
 *    The parameters specified here are those for for which we can't set up 
 *    reliable defaults, so we need to have the user set them.
 ***************************************************************************/
void xPID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd, int ControllerDirection)
{

    x_pid.myOutput = Output;
    x_pid.myInput = Input;
    x_pid.mySetpoint = Setpoint;
	x_pid.inAuto = false;

	xSetOutputLimits(-255, 255);				//default output limit corresponds to
												//the arduino pwm limits

    x_pid.SampleTime = 40;							//default Controller Sample Time is 0.1 seconds

    xSetControllerDirection(ControllerDirection);
    xSetTunings(Kp, Ki, Kd);

    x_pid.lastTime = millis()-x_pid.SampleTime;

}
void yPID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd, int ControllerDirection)
{
	
    y_pid.myOutput = Output;
    y_pid.myInput = Input;
    y_pid.mySetpoint = Setpoint;
	y_pid.inAuto = false;
	
	ySetOutputLimits(-255, 255);				//default output limit corresponds to
												//the arduino pwm limits

    y_pid.SampleTime = 40;							//default Controller Sample Time is 0.1 seconds

    ySetControllerDirection(ControllerDirection);
    ySetTunings(Kp, Ki, Kd);

    y_pid.lastTime = millis()-y_pid.SampleTime;
	
}

/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   x_pid Output needs to be computed.  returns true when the output is computed,
 *   false when nothing has been done.
 **********************************************************************************/ 
int xCompute(double * Input)
{
   unsigned long now,timeChange;
   if(!x_pid.inAuto) return false;
   x_pid.myInput = Input;
   now = millis();
   timeChange = (now - x_pid.lastTime);
   if(timeChange>=x_pid.SampleTime*0.5)
   {
      /*Compute all the working error variables*/
	  double input, error, dInput, output;
	  input = *x_pid.myInput;
      error = *x_pid.mySetpoint - input;
      x_pid.ITerm+= (x_pid.ki * error);
      if(x_pid.ITerm > x_pid.outMax) x_pid.ITerm= x_pid.outMax;
      else if(x_pid.ITerm < x_pid.outMin) x_pid.ITerm= x_pid.outMin;
      dInput = (input - x_pid.lastInput);
 
      /*Compute PID Output*/
      output = x_pid.kp * error + x_pid.ITerm- x_pid.kd * dInput;
      
	  if(output > x_pid.outMax) output = x_pid.outMax;
      else if(output < x_pid.outMin) output = x_pid.outMin;
	  output = output *0.5/255;
	  *x_pid.myOutput = -output;
	  
      /*Remember some variables for next time*/
      x_pid.lastInput = input;
      x_pid.lastTime = now;
	  return true;
   }
   else return false;
}

int yCompute(double * Input)
{
   unsigned long now,timeChange;
   if(!y_pid.inAuto) return false;
   y_pid.myInput = Input;
   now = millis();
   timeChange = (now - y_pid.lastTime);
   if(timeChange>=y_pid.SampleTime*0.5)
   {
      /*Compute all the working error variables*/
	  double input, error, dInput, output;
	  input = *y_pid.myInput;
      error = *y_pid.mySetpoint - input;
      y_pid.ITerm+= (y_pid.ki * error);
      if(y_pid.ITerm > y_pid.outMax) y_pid.ITerm= y_pid.outMax;
      else if(y_pid.ITerm < y_pid.outMin) y_pid.ITerm= y_pid.outMin;
      dInput = (input - y_pid.lastInput);

      /*Compute PID Output*/
      output = y_pid.kp * error + y_pid.ITerm- y_pid.kd * dInput;

	  if(output > y_pid.outMax) output = y_pid.outMax;
      else if(output < y_pid.outMin) output = y_pid.outMin;
	  output = output *0.5/255;
	  *y_pid.myOutput = -output;

      /*Remember some variables for next time*/
      y_pid.lastInput = input;
      y_pid.lastTime = now;
	  return true;
   }
   else return false;
}

int xComput(void)
{
	union{
	signed int all;
	unsigned char s[2];
	}data;
	char i, sci_send = 10;
	double input, error, dInput, output;
	unsigned long now, timeChange;
    if(!x_pid.inAuto) return false;
    now = millis();
    timeChange = (now - x_pid.lastTime);
    if(timeChange>=x_pid.SampleTime)
    {
      /*Compute all the working error variables*/
//		SCI5_Serial_Send(&sci_send, 1);
    while(sci5_receive_available() > 0)
    {
    SCI5_Serial_Receive(&data.s[i], 1);
    i++;
    }
   	  i=0;
	  input = (double)(data.all);
      error = *x_pid.mySetpoint - input;
      x_pid.ITerm+= (x_pid.ki * error);
      if(x_pid.ITerm > x_pid.outMax) x_pid.ITerm= x_pid.outMax;
      else if(x_pid.ITerm < x_pid.outMin)x_pid.ITerm= x_pid.outMin;
      dInput = (input - x_pid.lastInput);
 
      /*Compute PID Output*/
      output = x_pid.kp * error + x_pid.ITerm- x_pid.kd * dInput;
      
	  if(output > x_pid.outMax) output = x_pid.outMax;
      else if(output < x_pid.outMin) output = x_pid.outMin;
	  *x_pid.myOutput = output;
	  
      /*Remember some variables for next time*/
      x_pid.lastInput = input;
      x_pid.lastTime = now;
	//  Serial.print(*x_pid.myOutput);
	//  Serial.print(",");
	//  Serial.println(*myInput);
	  return true;
   }
   else return false;
}


int yComput(void)
{
	union{
	signed int all;
	unsigned char s[2];
	}data;
	char i, sci_send = 10;
	double input, error, dInput, output;
	unsigned long now, timeChange;
    if(!y_pid.inAuto) return false;
    now = millis();
    timeChange = (now - y_pid.lastTime);
    if(timeChange>=y_pid.SampleTime)
    {
      /*Compute all the working error variables*/
//		SCI5_Serial_Send(&sci_send, 1);
    while(sci5_receive_available() > 0)
    {
    SCI5_Serial_Receive(&data.s[i], 1);
    i++;
    }
   	  i=0;
	  input = (double)(data.all);
      error = *y_pid.mySetpoint - input;
      y_pid.ITerm+= (y_pid.ki * error);
      if(y_pid.ITerm > y_pid.outMax) y_pid.ITerm= y_pid.outMax;
      else if(y_pid.ITerm < y_pid.outMin)y_pid.ITerm= y_pid.outMin;
      dInput = (input - y_pid.lastInput);

      /*Compute PID Output*/
      output = y_pid.kp * error + y_pid.ITerm- y_pid.kd * dInput;

	  if(output > y_pid.outMax) output = y_pid.outMax;
      else if(output < y_pid.outMin) output = y_pid.outMin;
	  *y_pid.myOutput = output;

      /*Remember some variables for next time*/
      y_pid.lastInput = input;
      y_pid.lastTime = now;
	//  Serial.print(*y_pid.myOutput);
	//  Serial.print(",");
	//  Serial.println(*myInput);
	  return true;
   }
   else return false;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted. 
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/ 
void xSetTunings(double Kp, double Ki, double Kd)
{
	double SampleTimeInSec;
   if (Kp<0 || Ki<0 || Kd<0) return;
 
   x_pid.dispKp = Kp; x_pid.dispKi = Ki; x_pid.dispKd = Kd;
   
   SampleTimeInSec = ((double)x_pid.SampleTime)/1000;
   x_pid.kp = Kp;
   x_pid.ki = Ki * SampleTimeInSec;
   x_pid.kd = Kd / SampleTimeInSec;
 
  if(x_pid.controllerDirection ==REVERSE)
   {
      x_pid.kp = (0 - x_pid.kp);
      x_pid.ki = (0 - x_pid.ki);
      x_pid.kd = (0 - x_pid.kd);
   }
}

void ySetTunings(double Kp, double Ki, double Kd)
{
	double SampleTimeInSec;
   if (Kp<0 || Ki<0 || Kd<0) return;

   y_pid.dispKp = Kp; y_pid.dispKi = Ki; y_pid.dispKd = Kd;

   SampleTimeInSec = ((double)y_pid.SampleTime)/1000;
   y_pid.kp = Kp;
   y_pid.ki = Ki * SampleTimeInSec;
   y_pid.kd = Kd / SampleTimeInSec;

  if(y_pid.controllerDirection ==REVERSE)
   {
      y_pid.kp = (0 - y_pid.kp);
      y_pid.ki = (0 - y_pid.ki);
      y_pid.kd = (0 - y_pid.kd);
   }
}
/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed	
 ******************************************************************************/
void xSetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)x_pid.SampleTime;
      x_pid.ki *= ratio;
      x_pid.kd /= ratio;
      x_pid.SampleTime = (unsigned long)NewSampleTime;
   }
}

void ySetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)y_pid.SampleTime;
      y_pid.ki *= ratio;
      y_pid.kd /= ratio;
      y_pid.SampleTime = (unsigned long)NewSampleTime;
   }
}
/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void xSetOutputLimits(double Min, double Max)
{
   if(Min >= Max) return;
   x_pid.outMin = Min;
   x_pid.outMax = Max;
 
   if(x_pid.inAuto)
   {
	   if(*x_pid.myOutput > x_pid.outMax) *x_pid.myOutput = x_pid.outMax;
	   else if(*x_pid.myOutput < x_pid.outMin) *x_pid.myOutput = x_pid.outMin;
	 
	   if(x_pid.ITerm > x_pid.outMax) x_pid.ITerm= x_pid.outMax;
	   else if(x_pid.ITerm < x_pid.outMin) x_pid.ITerm= x_pid.outMin;
   }
}


void ySetOutputLimits(double Min, double Max)
{
   if(Min >= Max) return;
   y_pid.outMin = Min;
   y_pid.outMax = Max;

   if(y_pid.inAuto)
   {
	   if(*y_pid.myOutput > y_pid.outMax) *y_pid.myOutput = y_pid.outMax;
	   else if(*y_pid.myOutput < y_pid.outMin) *y_pid.myOutput = y_pid.outMin;

	   if(y_pid.ITerm > y_pid.outMax) y_pid.ITerm= y_pid.outMax;
	   else if(y_pid.ITerm < y_pid.outMin) y_pid.ITerm= y_pid.outMin;
   }
}
/* SetMode(...)****************************************************************
 * Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
 * when the transition from manual to auto occurs, the controller is
 * automatically initialized
 ******************************************************************************/ 
void xSetMode(int Mode)
{
    int newAuto = (Mode == AUTOMATIC);
    if(newAuto == !x_pid.inAuto)
    {  /*we just went from manual to auto*/
        xInitialize();
    }
    x_pid.inAuto = newAuto;
}

void ySetMode(int Mode)
{
    int newAuto = (Mode == AUTOMATIC);
    if(newAuto == !y_pid.inAuto)
    {  /*we just went from manual to auto*/
        yInitialize();
    }
    y_pid.inAuto = newAuto;
}
 
/* Initialize()****************************************************************
 *	does all the things that need to happen to ensure a bumpless transfer
 *  from manual to automatic mode.
 ******************************************************************************/ 
void xInitialize(void)
{
   x_pid.ITerm = *x_pid.myOutput;
   x_pid.lastInput = *x_pid.myInput;
   if(x_pid.ITerm > x_pid.outMax) x_pid.ITerm = x_pid.outMax;
   else if(x_pid.ITerm < x_pid.outMin) x_pid.ITerm = x_pid.outMin;
}

void yInitialize(void)
{
   y_pid.ITerm = *y_pid.myOutput;
   y_pid.lastInput = *y_pid.myInput;
   if(y_pid.ITerm > y_pid.outMax) y_pid.ITerm = y_pid.outMax;
   else if(y_pid.ITerm < y_pid.outMin) y_pid.ITerm = y_pid.outMin;
}
/* SetControllerDirection(...)*************************************************
 * The PID will either be connected to a DIRECT acting process (+Output leads
 * to +Input) or a REVERSE acting process(+Output leads to -Input.)  we need to
 * know which one, because otherwise we may increase the output when we should
 * be decreasing.  This is called from the constructor.
 ******************************************************************************/
void xSetControllerDirection(int Direction)
{
   if(x_pid.inAuto && Direction !=x_pid.controllerDirection)
   {
	  x_pid.kp = (0 - x_pid.kp);
      x_pid.ki = (0 - x_pid.ki);
      x_pid.kd = (0 - x_pid.kd);
   }   
   x_pid.controllerDirection = Direction;
}

void ySetControllerDirection(int Direction)
{
   if(y_pid.inAuto && Direction !=y_pid.controllerDirection)
   {
	  y_pid.kp = (0 - y_pid.kp);
      y_pid.ki = (0 - y_pid.ki);
      y_pid.kd = (0 - y_pid.kd);
   }
   y_pid.controllerDirection = Direction;
}
/* Status Funcions*************************************************************
 * Just because you set the Kp=-1 doesn't mean it actually happened.  these
 * functions query the internal state of the PID.  they're here for display
 * purposes.  this are the functions the PID Front-end uses for example
 ******************************************************************************/
