#pragma once
/*! \file prgLED.h
 * \brief This file defines the prgLED class
 */
#include "prgParent.h"

/*!
 * \class LEDStatesMachine
 * \brief This program informatoin is specific for the common LED state information
 */
class LEDStatesMachine : public parStatesMachine
{
public:
	int delaySecs;		//!< The number of seconds to delay before moving to the next state
	int delayMS;		//!< The number of milliseconds to delay before moving to the next state
	int pinState;		//!< Current PIN state
};

/*!
 * \brief This is the State Machine Program reader for LEDs
 */
class prgLED : public prgParent
{
public:
	std::vector<LEDStatesMachine> prgStates;						//!< This is the state program information

public:
	virtual void setValues(const char* pName, int dType) override;	//!< This sets up the values based on whatever is in the configuration file

	LEDStatesMachine *FindFirstProgramEntry();						//!< This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
	LEDStatesMachine* FindNextProgramEntry(LEDStatesMachine *cVal);	//!< This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
};

