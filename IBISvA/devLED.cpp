/*! \file devLED.cpp
 * \brief This file contains the implementation code for the devLED class
 */
#include "devLED.h"
#include <wiringPi.h>
#include <iostream>

//! In order to help with debugging, it will only be turned on per module instead of globally
/*! That way we can focus on only the area in question, just comment this out when done */
#define DBGMOD

//----------------------------------------------------------------------------------------
// This class is used for regular standard LED lights
//----------------------------------------------------------------------------------------

//! This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
devLED::devLED() : devParent()
{
#ifdef DBGMOD
	std::cout << "Start -> "; CallOut();
	std::cout << piBoardRev() << std::endl;
#endif
	bWrappedUp = false;

	// Initialize the State Machine
	curState = LEDStates::Unknown;
	nextState = LEDStates::Unknown;
}

//! This is the destructor and will clean up anything that was allocated while running
devLED::~devLED()
{
	WrapUp();
}

//! This sets up the values based on whatever is in the configuration file
/*!
 * \param dID This is the device ID number
 * \param dName This is the name of the device
 * \param pinNumber This is the PIN number used for communicating with the device
 * \param modeValue This will specify if this is a INPUT device or OUTPUT device
 * \param iState This is the initial state for this device when initializing
 * \param stateXNL This is the name of the State Machine Program file
 */
void devLED::setValues(int dID, const char* dName, int pinNumber, const char* modeValue, const char* iState,const char* stateXML)
{
	Pin = pinNumber;
	std::string inStr("INPUT");
	std::string hStr("HIGH");

	devName = dName;
	PinMode =((inStr.compare(modeValue) == 0) ? PinMode = PinModes::PinInput : PinMode = PinModes::PinOutput);
	initState = ((hStr.compare(iState) == 0) ? HIGH : LOW);
	prgName = stateXML;
	devID = dID;
#ifdef DBGMOD
	std::cout << "Device ID: " << devID << " -> " << prgName << std::endl;
#endif
}

//! This function performs anything that needs to be done before closing this device
void devLED::WrapUp()
{
	if (bWrappedUp == false)
	{
		bWrappedUp = true;
#ifdef DBGMOD
		std::cout << devName << " " << prgName << " Stop -> "; CallOut();
#endif
		// Let's force the LED to the initial state if it's an output device
		if (PinMode == PinModes::PinOutput)
		{
			::digitalWrite(Pin, initState);
		}
	}
}

//! This structure is used for translating the machine state to a string
static struct
{
	LEDStates sVal;
	const char* rcStr;
} sName[] = {
	{ LEDStates::LEDInit, "LEDStates::LEDInit" },
	{ LEDStates::Prg_Pin, "LEDStates::Prg_Pin" },
	{ LEDStates::Unknown, "LEDStates::Unknown" },
	{ LEDStates::Unknown, NULL                 }
};

//! This function returns the state name
/*!
 * \param stateVal This is the state value that will be changed to a string
 * \return string value of the name of the state
 */
const char* devLED::GetStateName(LEDStates stateVal)
{
	for (register int i = 0; sName[i].rcStr != NULL; i++)
		if (stateVal == sName[i].sVal) return sName[i].rcStr;
	return "Bad Value";
}

//! This function handles the state machine for this device
void devLED::HandleStates()
{
	if (curState==LEDStates::Unknown) InitLed_Start();

	if (this->endFun!=NULL)
	{
		if ((this->*endFun)())
		{
			curState = nextState;
			if (this->startFun != NULL) (this->*startFun)();
		}
	}
	else
	{
		std::cout << "Endless loop" << std::endl;
	}
}

//! This function sets up the current state information, this allows us to send notifications and setup statistics, etc later on
void devLED::SetCurState(LEDStates cState, bool (devLED::* eFun)())
{
#ifdef DBGMOD
	//std::cout << devName << " " << prgName << " " << GetStateName(cState) << " -> Start" << std::endl;
	std::cout << devName << " " << prgName << " " << " -> Start" << std::endl;
	if (curPrg == NULL)
	{
		std::cout << "Error: curPrg is NULL!" << std::endl;
		if (ledPrg != NULL)
		{
			std::cout << "However the program is not NULL, let's try something just in case" << std::endl;
			curPrg = ledPrg->FindFirstProgramEntry();
		}
		std::cout << curPrg->name << std::endl;
	}
#endif
	curState = cState;
	endFun = eFun;
}

//! This function sets up the next state information, this allows us to send notifications and setup statistics, etc later on
void devLED::SetNextState(LEDStates cState, void (devLED::* sFun)())
{
#ifdef DBGMOD
	std::cout << devName << " " << prgName << " " << GetStateName(curState) << " -> End" << std::endl;
#endif
	nextState = cState;
	startFun = sFun;
}

//! This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
void devLED::FindFirstProgramEntry()
{
	curPrg = ledPrg->FindFirstProgramEntry();
#ifdef DBGMOD
	if (curPrg == NULL)
	{
		std::cout << "FindFirstProgramEntry() == NULL" << std::endl;
	}
#endif
}

//! This function will go to the next program entry
void devLED::FindNextProgramEntry()
{
	curPrg = ledPrg->FindNextProgramEntry(curPrg);
#ifdef DBGMOD
	if (curPrg == NULL)
	{
		std::cout << "FindNextProgramEntry() == NULL" << std::endl;
	}
#endif
}

//! Initialize the state of the GPIO port used for the LED
void devLED::InitLed_Start()
{
	if (PinMode == PinModes::PinOutput) ::digitalWrite(Pin, initState);
	::pinMode(Pin, (int)PinMode);
	SetCurState(LEDStates::LEDInit, &devLED::InitLed_End);
}

//! Since this state really doesn't have to wait for anything, just immediately move to the next state
bool devLED::InitLed_End()
{
	SetNextState(LEDStates::Prg_Pin, &devLED::Prg_Pin_Start);
#ifdef DBGMOD
	std::cout << devName << " " << prgName << " Ok Trying" << std::endl;
#endif
	FindFirstProgramEntry();
#ifdef DBGMOD
	std::cout << "done " << std::endl;
#endif
	return true;
}

//! Now setting up the program version of the state machine
void devLED::Prg_Pin_Start()
{
#ifdef DBGMOD
	std::cout << devName << " " << prgName << " digitalWrite(" << Pin << "," << curPrg->pinState << ");" << std::endl;
#endif
	::digitalWrite(Pin, curPrg->pinState);	// led on
	markStart();
	SetCurState(LEDStates::Prg_Pin, &devLED::Prg_Pin_End);
#ifdef DBGMOD
	std::cout << devName << " " << prgName << " Delay Seconds: " << curPrg->delaySecs << " -> MS: " << curPrg->delayMS << std::endl;
#endif
}

//! The end function for this state
bool devLED::Prg_Pin_End()
{
	if (chkDelay(curPrg->delaySecs, curPrg->delayMS))
	{
		FindNextProgramEntry();
		SetNextState(LEDStates::Prg_Pin, &devLED::Prg_Pin_Start);
		return true;
	}
	return false;
}
