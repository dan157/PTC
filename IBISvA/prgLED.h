#pragma once

#include "prgParent.h"

// This program informatoin is specific for the common LED state information
class LEDStatesMachine : public parStatesMachine
{
public:
	int delaySecs;
	int delayMS;
	int pinState;
};

class prgLED : public prgParent
{
public:
	// This is the state program information
	std::vector<LEDStatesMachine> prgStates;

public:
	// This sets up the values based on whatever is in the configuration file
	virtual void setValues(const char* pName, int dType) override;

	// This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
	LEDStatesMachine *FindFirstProgramEntry();
	// This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
	LEDStatesMachine* FindNextProgramEntry(LEDStatesMachine *cVal);
};

