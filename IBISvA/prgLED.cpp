/*! \file prgLED.cpp
 * \brief This file implements the prgLED class
 */
#include "prgLED.h"

// In order to help with debugging, it will only be turned on per module instead of globally
// That way we can focus on only the area in question, just comment this out when done
#define DBGMOD

//----------------------------------------------------------------------------------------
// This class is used for defining the state machines for an LED light
//----------------------------------------------------------------------------------------

// This sets up the values based on whatever is in the configuration file
void prgLED::setValues(const char* pName, int dType)
{
	deviceType = dType;
	prgName = pName;
}

// This function will find the entry with the lowest ID to determine the first entry and then return a pointer to that, if none it will return NULL
LEDStatesMachine *prgLED::FindFirstProgramEntry()
{
	int maxStates = prgStates.size();
	int lastID = 10000;
	int iVal;

	LEDStatesMachine* rState = NULL;

	if (maxStates)
	{
		for (int i = 0; i < maxStates; i++)
		{
			if (prgStates.at(i).stateNum < lastID)
			{
				iVal = i;
				lastID = prgStates.at(i).stateNum;
			}
		}
		rState = &prgStates.at(iVal);
	}
	return (rState);
}

// This functin will go to the next program entry
LEDStatesMachine* prgLED::FindNextProgramEntry(LEDStatesMachine *cVal)
{
	int maxStates = prgStates.size();

	if (maxStates)
	{
		for (int i = 0; i < maxStates; i++)
		{
			if (prgStates.at(i).stateNum == cVal->nextState)
			{
				return (&prgStates.at(i));
			}
		}
	}
	std::cout << "Error: State Entry Not Found! " << prgName << std::endl;
	return NULL;
}

