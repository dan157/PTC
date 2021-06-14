//------------------------------------------------------------------------------------
// This is the first version of a state machine, used to demonstrate how a State
// machine works.
//------------------------------------------------------------------------------------
#include <wiringPi.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "State1.h"

//------------------------------------------------------------------------------------
// Define the behaviors of this machine state
//------------------------------------------------------------------------------------
enum { st_Unknown = -1, st_LEDInit=0, st_LEDOn=1, st_LEDOff=2, st_Delay=3, numStates=4 };

struct s_stateType States[numStates+1] =
{
	// State	  Function Name,	Init Function	 Completion
	{ st_LEDInit, 	"Init Led",		InitLed_Start, InitLed_End },
	{ st_LEDOn,   	"LED On",		LEDOn_Start,   LEDOn_End   },
	{ st_LEDOff,  	"LED Off",		LEDOff_Start,  LEDOff_End  },
	{ st_Delay,   	"Delay",		Delay_Start,   Delay_End   },
	{ st_Unknown, "", NULL, NULL }
};

struct s_behavior Behavior[numStates] =
{
	// State,		Next,			Error
	{ st_LEDInit,		st_LEDOn,		st_LEDInit },
	{ st_LEDOn,		st_LEDOff,		st_LEDInit },
	{ st_LEDOff,		st_Delay,		st_LEDInit },
	{ st_Delay,		st_LEDOn,		st_LEDInit }
};

//------------------------------------------------------------------------------------
// structure to keep track of different items needed for the states
//------------------------------------------------------------------------------------
struct
{
	int curState;
	int nextState;
	struct timespec timeStart;
	struct timespec timeNow;
	clockid_t clk_id;
} dev1;

//------------------------------------------------------------------------------------
// The following functions define the behavior of the device
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// This function marks the start of a "wait" delay
//------------------------------------------------------------------------------------
void markStart(void)
{
	int result = clock_gettime(dev1.clk_id,&dev1.timeStart);
}

//------------------------------------------------------------------------------------
// This function will check to see if the requested elapsed time has exceeded
// If it has, then it will return TRUE, otherwise FALSE
//------------------------------------------------------------------------------------
bool chkDelay(unsigned int chkSec,time_t chkMS)
{
	time_t chkVal;
	if (clock_gettime(dev1.clk_id, &dev1.timeNow) != 0)
	{
		// Maybe we should add more error handling later on
		return TRUE;
	}
	// First check to see if the seconds has been reached
	if ((dev1.timeNow.tv_sec-dev1.timeStart.tv_sec) == chkSec)
	{
		// If so then let's check the milliseconds, right now this code is not expected
		// to be able to handle nanoseconds, so let's convert the test to ms
		// MS = 1,000 per second
		// NS = 1,000,000,000 per second
		chkVal = chkMS * 1000000;
		if ((dev1.timeNow.tv_nsec-dev1.timeStart.tv_nsec) >= chkVal) return TRUE;
	}
	return ((dev1.timeNow.tv_sec-dev1.timeStart.tv_sec) > chkSec);
}

//------------------------------------------------------------------------------------
// Initialize the state of the GPIO port used for the LED
//------------------------------------------------------------------------------------
void InitLed_Start(void)
{
	pinMode(LedPin, OUTPUT);
}

//------------------------------------------------------------------------------------
// Since this state really doesn't have to wait for anything, just immediately move to the next state
//------------------------------------------------------------------------------------
bool InitLed_End(void)
{
	dev1.nextState = Behavior[dev1.curState].nextState;
	return TRUE;
}

//------------------------------------------------------------------------------------
// This function will turn on the LED light
//------------------------------------------------------------------------------------
void LEDOn_Start(void)
{
	digitalWrite(LedPin,HIGH);	// led on
	markStart();
}

//------------------------------------------------------------------------------------
// This function determines if turning on the LED light is completed and ready for the
// next state.
//------------------------------------------------------------------------------------
bool LEDOn_End(void)
{
	if (chkDelay(1,0))
	{
		dev1.nextState = Behavior[dev1.curState].nextState;
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------------
// This function will turn off the LED light
//------------------------------------------------------------------------------------
void LEDOff_Start(void)
{
	digitalWrite(LedPin,LOW);	// led on
	markStart();
}

//------------------------------------------------------------------------------------
// This function determines if turning on the LED light is completed and ready for the
// next state.
//------------------------------------------------------------------------------------
bool LEDOff_End(void)
{
	if (chkDelay(1,0))
	{
		dev1.nextState = Behavior[dev1.curState].nextState;
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------------
// This function only performs a delay, no additional tasks
//------------------------------------------------------------------------------------
void Delay_Start(void)
{
	markStart();
}

//------------------------------------------------------------------------------------
// This function goes to the next state after the delay time has expired
//------------------------------------------------------------------------------------
bool Delay_End(void)
{
	if (chkDelay(0,500))
	{
		dev1.nextState = Behavior[dev1.curState].nextState;
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------------
// This is the main function and performs the endless loop
//------------------------------------------------------------------------------------
int main(void)
{
	if (wiringPiSetup() == -1)
	{
		printf("setup wiringPi failed!\n");
		return -1;
	}
	dev1.clk_id = CLOCK_REALTIME;
	dev1.curState = st_Unknown;
	dev1.nextState = st_Unknown;

	while (1)
	{
		if (dev1.curState == st_Unknown)
		{
			dev1.curState = st_LEDInit;
			printf("State: %d - %s\n",dev1.curState,States[dev1.curState].funName);
			(*States[dev1.curState].startFun)();
		}
		else
		{
			if ((*States[dev1.curState].endFun)())
			{
				dev1.curState = dev1.nextState;
				printf("State: %d - %s\n",dev1.curState,States[dev1.curState].funName);
				(*States[dev1.curState].startFun)();
			}
			else
			{
				// If we need to process anything while waiting for the next stage
				// we can place it here
			}
		}
	}
}
