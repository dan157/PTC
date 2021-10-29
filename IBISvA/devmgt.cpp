/*! \file devmgt.cpp
 * \brief This file contains the implementation code for the devmgt class
 */
#include "devmgt.h"
#include "pugixml.hpp"
#include "devLED.h"
#include <string>


 //! In order to help with debugging, it will only be turned on per module instead of globally
 /*! That way we can focus on only the area in question, just comment this out when done */
#define DBGMOD

//----------------------------------------------------------------------------------------
// This class is the parent class that all Devices will be a child of, as we add more
// capabilities, if it's something that is reusable then it will be in the parent above
// so several device types can share some attributes saving us to have to code the same
// thing over and over again.
//----------------------------------------------------------------------------------------

// This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
devmgt::devmgt()
{
#ifdef DBGMOD
	std::cout << "Start -> "; CallOut();
#endif

	// Let's initialize the devices just to make sure everything is as we expect it
	numDevices = 0;
	for (int i = 0; i < maxDevices; i++) { DevArray[i] = nullptr; }
}

// This is the destructor and will clean up anything that was allocated while running
devmgt::~devmgt()
{
	// Let's release any devices that are assigned
	for (int i = 0; i < numDevices; i++)
	{
		if (DevArray[i] != nullptr)
		{
			devParent* p = DevArray[i];
			p->WrapUp();
			
			delete p;
			DevArray[i] = nullptr;
		}
#ifdef DBGMOD
		else
		{
			std::cout << "Strange, pointer is NULL" << std::endl;
		}
#endif
	}

#ifdef DBGMOD
	std::cout << "Stop  -> "; CallOut();
#endif
}

//!< This function will check to see if there are any devices to add, if they exist it will be added
/*!
 * \param pInfo This is a pointer to the program management class allowing us to link the two together
 */
int devmgt::findDevices(PrgMgmt* pInfo)
{
	DeviceType test1;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("/etc/IBIS/conf/system.xml");

	SystemName = doc.child("System").attribute("name").value();

#ifdef DBGMOD
	std::cout << "System Name: " << SystemName << std::endl;
#endif

	pugi::xml_node devices = doc.child("System").child("Devices");

	for (pugi::xml_node device = devices.first_child(); device; device = device.next_sibling())
	{
		if (numDevices < maxDevices)
		{
#ifdef DBGMOD
			for (pugi::xml_attribute attr = device.first_attribute(); attr; attr = attr.next_attribute())
			{
				std::cout << " " << attr.name() << "=" << attr.value();
			}
			std::cout << std::endl;
#endif
			if (numDevices < maxDevices)
			{
				// Just being extra careful not to exceed the max device count
				std::cout << numDevices << "  "  << device.attribute("type").value() << std::endl;
				test1 = GetDevType(device.attribute("type").value());
				switch (test1)
				{
				case DeviceType::LED:
					devParent* p = new devLED();
					DevArray[numDevices] = p;
					p->setValues(device.attribute("id").as_int(), device.attribute("name").value(), device.attribute("PIN").as_int(), device.attribute("INITMODE").value(), device.attribute("INITSTATE").value(), device.attribute("STATES").value());
					p->thisDev = (int)test1;
					devLED* p2 = (devLED*)p;
					p2->ledPrg = (prgLED*)pInfo->FindMachine(p->prgName.c_str());
					if (p2->ledPrg == NULL)
					{
						std::cout << "Error Unable to find program name: " << p->prgName << " for device " << p->devName << std::endl;
					}
					else
					{
						std::cout << "Device Name: " << p->devName << " was assigned program " << p->prgName << std::endl;
					}
					numDevices++;
					break;
				}
			}
		}
	}

	return 0;
}

// This function Handles the states for all devices
void devmgt::HandleStates()
{
	for (int i = 0; i < numDevices; i++)
	{
		if (DevArray[i] != nullptr)
		{
			devParent* p = DevArray[i];

			p->HandleStates();
		}
	}
}
