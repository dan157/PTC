#pragma once
/*! \file devLED.h
 * \brief This file defines the devLED class
 */
#include "devParent.h"
#include "prgLED.h"
#include <wiringPi.h>
#include <string>
#include <vector>

/*! These are all of the states possible currently for the LED State Machine, additional states will be defined later on as needed */
enum class LEDStates
{
	LEDInit,		/*!< This state initializes the LED */
	Prg_Pin,		/*!< This state is specifically used to turn either off or on the LED */
	Unknown = -1	/*!< This is the unknown state which basically just has the system go back to the LEDInit state */
};

/*! Currently we only have INPUT and OUTPUT types of PINs for the LED devices, this can change in the future as we work with different LED devices */
enum class PinModes
{
	PinInput = INPUT,	/*!< This device will allow us to read from it */
	PinOutput = OUTPUT	/*!< This device will allow us to write to it */
};

//! \brief This class is used for regular standard LED lights
class devLED : public devParent
{
	bool bWrappedUp;				//!< This is just a flag to check to see if the destructor has already been called or not
	LEDStates curState;				//!< This is the current state value
	LEDStates nextState;			//!< This is the next state value

	void (devLED::* startFun)();	//!< This is the pointer to the start function for the current state
	bool (devLED::* endFun)();		//!< This is the pointer to the end function for the current state

	int Pin;						//!< This is the PIN that is specific for this LED device
	PinModes PinMode;				//!< This is the PinMode of the current device
	int initState;					//!< This is the initial mode of this device when initializing

public:
	LEDStatesMachine* curPrg;		//!< This is a pointer to the current state
	prgLED* ledPrg;					//!< This is a pointer to the state program assigned to this device

public:
	devLED();						//!< This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
	~devLED();						//!< This is the destructor and will clean up anything that was allocated while running

	virtual void setValues(int dID, const char *dName, int pinNumber,const char *modeValue, const char* initState,const char* stateXML) override;	//!< This sets up the values based on whatever is in the configuration file

public:
	virtual void CallOut() override { std::cout << "devLED" << std::endl; }	//!< Used for debugging
	virtual void WrapUp() override;
	virtual void HandleStates() override;

private:
	const char* GetStateName(LEDStates stateVal);
	void SetCurState(LEDStates cState, bool (devLED::* eFun)());
	void SetNextState(LEDStates cState, void (devLED::* sFun)());

	void FindFirstProgramEntry();
	void FindNextProgramEntry();

private:
	// State Machine functions
	void InitLed_Start();
	bool InitLed_End();
	void Prg_Pin_Start();
	bool Prg_Pin_End();
};

