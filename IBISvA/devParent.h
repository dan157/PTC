#pragma once
#include <iostream>
#include "prgParent.h"

/*! \file devParent.h
 * \brief This file defines the devParent class
 */

/*! This is the current known list of device types the system is programmed for at this time */
enum class DeviceType
{
	LED = 0,		//!< LED Lights
	Unknown = -1	//!< Unknown device type
};

/*!
 * \class TypeConv
 * \brief This typedef allows us to convert from the DeviceType to the String
 */
typedef struct
{
	const char* name;	//!< The string name of the device
	DeviceType value;	//!< The DeviceType value of the device
} TypeConv;

DeviceType GetDevType(const char* typeName);
const char* GetDevType(DeviceType typeVal);

//! This class defines the common values used for all state program information for every type of device
class parStatesMachine
{
public:
	int stateNum;		//!< The current state number
	int nextState;		//!< The next state value
	std::string name;	//!< The name of this state machine program
	char cmdCode;		//!< The command code defining the next state type
};

/*!
 * \class devParent
 * \brief This class is the baseline for all devices
 * 
 * This class is the parent class that all Devices will be a child of, as we add more capabilities, if it's something that is reusable then it will be in the parent above so several device types can share some attributes saving us to have to code the same thing over and over again.
 * 
 */
class devParent
{
	clockid_t clk_id;							//!< This is the clockid value
	struct timespec timeStart;					//!< This allows us to mark the start time for the delay functions
	struct timespec timeNow;					//!< This is used to compare the current time against the timeStart to determine if enough time has elapsed
public:
	devParent() { clk_id = CLOCK_REALTIME; }	//!< This is the constructor
	std::string prgName;						//!< This is the state filename
	int thisDev;								//!< This is the device ID value of the current device
	int devID;									//!< This is the device ID of the current device
	std::string devName;						//!< This is the name of the current device

	virtual void CallOut() { std::cout << "devParent" << std::endl; }				//!< Used for debugging
	virtual void WrapUp() { std::cout << "Wrapping Up devParent" << std::endl; }	//!< Used to wrap up the class before exiting the program
	virtual void HandleStates() { std::cout << "devParent shouldn't be handling the States!" << std::endl; }	//!< This is the state machine, which whould be implemented in the device specific class only
	virtual void setValues(int dID, const char* dName, int pinNumber, const char* modeValue, const char* initState,const char* stateXML) { std::cout << "devParent shouldn't be handling the init values!" << std::endl; } //!< This really should only be defined in the device specific class

public:
	void markStart() { clock_gettime(clk_id, &timeStart); }		//!< This function marks the start time
	bool chkDelay(unsigned int chkSec, time_t chkMS);			//!< This function will check to see if the requested elapsed time has exceeded.  If it has, then it will return TRUE, otherwise FALSE
};

