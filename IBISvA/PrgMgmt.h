#pragma once

#include "prgParent.h"

#define maxPrograms	100

class PrgMgmt : public prgParent
{
	// The number of programs that were found and loaded
	int numPrograms;
	// The array of devices that were found
	prgParent* PrgArray[maxPrograms];

public:
	// This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
	PrgMgmt();

	// This is the destructor and will clean up anything that was allocated while running
	~PrgMgmt();

public:
	// Used for debugging
	virtual void CallOut() override { std::cout << "PrgMgmt" << std::endl; }
	// This function will check to see if there are any devices to add, if they exist it will be added
	int findPrograms();
public:
	// This function searches for the State Machine for a program
	prgParent* FindMachine(const char* progName);
private:
	// This function reads in the State Machine information as needed
	void ReadStateMachine(const char* fileName);
private:
	std::string ProgramName;
};

