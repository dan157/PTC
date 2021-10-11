#pragma once

#include <iostream>

#include "prgParent.h"

enum class DeviceType
{
	LED = 0,
	Unknown = -1
};

typedef struct
{
	const char* name;
	DeviceType value;
} TypeConv;

DeviceType GetDevType(const char* typeName);
const char* GetDevType(DeviceType typeVal);

// This class defines the common values used for all state program information for every type of device
class parStatesMachine
{
public:
	int stateNum;
	int nextState;
	std::string name;
	char cmdCode;
};

//----------------------------------------------------------------------------------------
// This class is the parent class that all Devices will be a child of, as we add more
// capabilities, if it's something that is reusable then it will be in the parent above
// so several device types can share some attributes saving us to have to code the same
// thing over and over again.
//----------------------------------------------------------------------------------------
class devParent
{
	clockid_t clk_id;
	struct timespec timeStart;
	struct timespec timeNow;
public:
	devParent() { clk_id = CLOCK_REALTIME; }
	// This is the state filename
	std::string prgName;
	int thisDev;
	int devID;
	std::string devName;
	//prgParent* parInfo;

	virtual void CallOut() { std::cout << "devParent" << std::endl; }
	virtual void WrapUp() { std::cout << "Wrapping Up devParent" << std::endl; }
	virtual void HandleStates() { std::cout << "devParent shouldn't be handling the States!" << std::endl; }
	virtual void setValues(int dID, const char* dName, int pinNumber, const char* modeValue, const char* initState,const char* stateXML) { std::cout << "devParent shouldn't be handling the init values!" << std::endl; }

public:
	// This function marks the start time
	void markStart() { clock_gettime(clk_id, &timeStart); }
	// This function will check to see if the requested elapsed time has exceeded.  If it has, then it will return TRUE, otherwise FALSE
	bool chkDelay(unsigned int chkSec, time_t chkMS);
};

