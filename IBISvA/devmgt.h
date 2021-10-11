#pragma once

#include "devParent.h"
#include "PrgMgmt.h"

//----------------------------------------------------------------------------------------
// This class is used to manage several devices.  At some point this will be created for
// each microcontroller individually, but for now it will all be handled on the single
// Raspberry Pi.
// There will be two approaches made by this application:
// 1) Single Board Computer -> Managing all devices and sub-devices
// 2) Multiple Controllers -> Each controller will handle it's own set of devices and will
//	  report back to the main Raspberry Pi controller.  There will be various versions of
//	  communications between each device based on the hardware available for the unit
//	  in question some will have more latency then others which means realtime
//	  information is only as good as the respond time for each.
//----------------------------------------------------------------------------------------

// For now let's device 10 as the max number of devices this module will support.  Obviously this can change to any value you want.
#define maxDevices	100

// class devmgt is used to manage the devices either directly on the computer, or indirectly through communications
class devmgt: public devParent
{
	// The number of devices that was found and initialized
	int numDevices;
	// The array of devices that were found
	devParent* DevArray[maxDevices];

public:
	// This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
	devmgt();

	// This is the destructor and will clean up anything that was allocated while running
	~devmgt();

	// This function Handles the states for all devices
	virtual void HandleStates() override;
public:
	// Used for debugging
	virtual void CallOut() override { std::cout << "devmgt" << std::endl; }

	// This function will check to see if there are any devices to add, if they exist it will be added
	int findDevices(PrgMgmt *pInfo);
private:
	std::string SystemName;
};

