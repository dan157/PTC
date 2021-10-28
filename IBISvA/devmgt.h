#pragma once

#include "devParent.h"
#include "PrgMgmt.h"

/*! \file devmgt.h
 * \brief This file defines the devmgt class
 */

#define maxDevices	100	//!< For now let's device 10 as the max number of devices this module will support.  Obviously this can change to any value you want.

/*!
 * \class devmgt
 * \brief This class is used to manage the devices either directly on the computer, or indirectly through communications
 * 
 * This class is used to manage several devices.  At some point this will be created for
 * each microcontroller individually, but for now it will all be handled on the single
 * Raspberry Pi.
 *
 * - There will be two approaches made by this application:
 *    -# Single Board Computer -> Managing all devices and sub-devices
 *    -# Multiple Controllers -> Each controller will handle it's own set of devices and will report back to the main Raspberry Pi controller.  There will be various versions of communications between each device based on the hardware available for the unit in question some will have more latency then others which means realtime information is only as good as the respond time for each
 */
class devmgt: public devParent
{
	int numDevices;							//!< The number of devices that was found and initialized
	devParent* DevArray[maxDevices];		//!< The array of devices that were found

public:
	devmgt();								//!< This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
	~devmgt();								//!< This is the destructor and will clean up anything that was allocated while running

	virtual void HandleStates() override;	//!< This function Handles the states for all devices
public:
	virtual void CallOut() override { std::cout << "devmgt" << std::endl; }	//!< Used for debugging

	int findDevices(PrgMgmt *pInfo);
private:
	std::string SystemName;					//!< This is the name of the system
};

