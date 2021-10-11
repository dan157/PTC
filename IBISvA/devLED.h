#pragma once
#include "devParent.h"
#include "prgLED.h"
#include <wiringPi.h>
#include <string>
#include <vector>

enum class LEDStates
{
	LEDInit,
	Prg_Pin,
	Unknown = -1
};

enum class PinModes
{
	PinInput = INPUT,
	PinOutput = OUTPUT
};

//
//Was able to find this statement when trying to figure out how the PINs are translated in wired mode.....
//
//The code change:-
//
//static int pinToGpio [] =
//{
//  17, 18, 21, 22, 23, 24, 25, 4, - From the Wiki – GPIO 0 through 7
//	0, 1, - SDA0, SCL0 – The I2C pins
//	8, 7, - SPI – CE0, CE1 —- was 7,8
//	10, 9, 11, - SPI – MOSI, MOSO, SCLK —- was 9,10,11
//	14, 15, - UART – Tx, Rx
//}

//----------------------------------------------------------------------------------------
// This class is used for regular standard LED lights
//----------------------------------------------------------------------------------------
class devLED : public devParent
{
	// This is just a flag to check to see if the destructor has already been called or not
	bool bWrappedUp;

	// This is the current state value
	LEDStates curState;
	// This is the next state value
	LEDStates nextState;

	void (devLED::* startFun)();
	bool (devLED::* endFun)();

	// This is the PIN that is specific for this LED device
	int Pin;
	PinModes PinMode;
	int initState;

public:
	// This is a pointer to the current state
	LEDStatesMachine* curPrg;
	// This is a pointer to the state program assigned to this device
	prgLED* ledPrg;

public:
	// This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
	devLED();
	// This is the destructor and will clean up anything that was allocated while running
	~devLED();

	// This sets up the values based on whatever is in the configuration file
	virtual void setValues(int dID, const char *dName, int pinNumber,const char *modeValue, const char* initState,const char* stateXML) override;

public:
	// Used for debugging
	virtual void CallOut() override { std::cout << "devLED" << std::endl; }
	// Used to make sure things are closed correctly
	virtual void WrapUp() override;
	// This function handles the state machine for this device
	virtual void HandleStates() override;

private:
	// Return the name of the current State
	const char* GetStateName(LEDStates stateVal);
	void SetCurState(LEDStates cState, bool (devLED::* eFun)());
	void SetNextState(LEDStates cState, void (devLED::* sFun)());

	// This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
	void FindFirstProgramEntry();
	// This functin will go to the next program entry
	void FindNextProgramEntry();

private:
	// State Machine functions

	// Initialize the state of the GPIO port used for the LED
	void InitLed_Start();
	// Since this state really doesn't have to wait for anything, just immediately move to the next state
	bool InitLed_End();
	// This function will turn on the LED light
	void LEDOn_Start();
	// This function determines if turning on the LED light is completed and ready for the next state.
	bool LEDOn_End();
	// This function will turn off the LED light
	void LEDOff_Start();
	// This function determines if turning on the LED light is completed and ready for the next state.
	bool LEDOff_End();
	// This function only performs a delay, no additional tasks
	void Delay_Start();
	// This function goes to the next state after the delay time has expired
	bool Delay_End();
	// Now setting up the program version of the state machine
	void Prg_Pin_Start();
	// The end function for this state
	bool Prg_Pin_End();
};

