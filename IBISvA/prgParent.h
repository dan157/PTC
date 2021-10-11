#pragma once

#include "devParent.h"
#include <iostream>
#include <vector>

//----------------------------------------------------------------------------------------
// This class is the parent class that all programs will be a child of, as we add more
// capabilities, if it's something that is reusable then it will be in the parent above
// so several device types can share some attributes saving us to have to code the same
// thing over and over again.
//----------------------------------------------------------------------------------------
class prgParent
{
public:
	std::string prgName;
	int deviceType;
	virtual void CallOut() { std::cout << "prgParent" << std::endl; }
	virtual void WrapUp() { std::cout << "Wrapping Up prgParent" << std::endl; }
	virtual void setValues(const char* pName, int dType) { std::cout << "devParent shouldn't be handling the init values!" << std::endl; }
	// This function returns if this is the program name that is being checked against
	bool IsProgram(const char* chkName) { return (prgName.compare(chkName) == 0); }
};

