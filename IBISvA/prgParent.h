#pragma once
/*! \file prgParent.h
 * \brief This file defines the prgParent class
 */
#include "devParent.h"
#include <iostream>
#include <vector>

/*!
 * \class prgParent
 * \brief This class is the parent class for all State Machine Programs
 * 
 * This class is the parent class that all programs will be a child of, as we add more capabilities, if it's something that is reusable then it will be in the parent above so several device types can share some attributes saving us to have to code the same thing over and over again
 */
class prgParent
{
public:
	std::string prgName;	//!< The name of the program
	int deviceType;			//!< The device type
	virtual void CallOut() { std::cout << "prgParent" << std::endl; }	//!< Used for debugging
	virtual void WrapUp() { std::cout << "Wrapping Up prgParent" << std::endl; }	//!< Performs cleanup before the system exits
	virtual void setValues(const char* pName, int dType) { std::cout << "devParent shouldn't be handling the init values!" << std::endl; } //!< Set the values, however this should be implemented in the a child class
	bool IsProgram(const char* chkName) { return (prgName.compare(chkName) == 0); }	//!< This function returns if this is the program name that is being checked against
};

