//------------------------------------------------------------------------------------
// This is the header file that contains definitions specific to the State1 module
//------------------------------------------------------------------------------------
#ifndef __State1__
#define __State1__

//------------------------------------------------------------------------------------
// This is the GPIO that is used for this example
//------------------------------------------------------------------------------------
#define LedPin 0

//------------------------------------------------------------------------------------
// Since this is C we will just pre-define the functions that will be used in the
// state machine
//------------------------------------------------------------------------------------
void InitLed_Start(void);
bool InitLed_End(void);
void LEDOn_Start(void);
bool LEDOn_End(void);
void LEDOff_Start(void);
bool LEDOff_End(void);
void Delay_Start(void);
bool Delay_End(void);

//------------------------------------------------------------------------------------
// Define the structure that will contain the state number, and the start and end
// functions for each state
//------------------------------------------------------------------------------------
struct s_stateType
{
	int stateVal;			// This is the state index value
	char funName[20];		// The name that we want to display when executing
	void (*startFun)(void);	// The function called to start this function
	bool (*endFun)(void);	// The function called to check if it's completed
};

//------------------------------------------------------------------------------------
// This structure is used to help define the behaviors that we want the device to
// perform
//------------------------------------------------------------------------------------
struct s_behavior
{
	int stateVal;		// Current State
	int nextState;		// Next State
	int errorState;		// If there's an error which state
};

#endif // __State1__

