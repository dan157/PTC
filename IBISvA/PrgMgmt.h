#pragma once
/*! \file PrgMgmt.h
 * \brief This file defines the PrgMgmt class
 */
#include "prgParent.h"

#define maxPrograms	100		//!< Current max number of programs to store, this is just a quick number and can be changed at anytime

/*!
 * \class PrgMgmt
 * \brief This class stores into memory the State Machine program based on XML
 */
class PrgMgmt : public prgParent
{
	int numPrograms;								//!< The number of programs that were found and loaded
	prgParent* PrgArray[maxPrograms];				//!< The array of devices that were found

public:
	PrgMgmt();										//!< This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
	~PrgMgmt();										//!< This is the destructor and will clean up anything that was allocated while running

public:
	virtual void CallOut() override { std::cout << "PrgMgmt" << std::endl; }	//!< Used for debugging
	int findPrograms();								//!< This function will check to see if there are any devices to add, if they exist it will be added

public:
	prgParent* FindMachine(const char* progName);	//!< This function searches for the State Machine for a program

private:
	void ReadStateMachine(const char* fileName);	//!< This function reads in the State Machine information as needed
	std::string ProgramName;						//!< This is the name of the program
};

