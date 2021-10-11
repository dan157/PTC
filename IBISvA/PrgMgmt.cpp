#include "PrgMgmt.h"
#include "prgLED.h"
#include "pugixml.hpp"
#include <string>
#include <cstring>
#include <dirent.h>

// In order to help with debugging, it will only be turned on per module instead of globally
// That way we can focus on only the area in question, just comment this out when done
#define DBGMOD

//----------------------------------------------------------------------------------------
// This class is the parent class that all Programs will be a child of, as we add more
// capabilities, if it's something that is reusable then it will be in the parent above
// so several device types can share some attributes saving us to have to code the same
// thing over and over again.
//----------------------------------------------------------------------------------------

// This is the constructor for this class, and performs the initialization processes that are needed to perform it's tasks while running.
PrgMgmt::PrgMgmt()
{
#ifdef DBGMOD
	std::cout << "Start -> "; CallOut();
#endif

	// Let's initialize the devices just to make sure everything is as we expect it
	numPrograms = 0;
	for (int i = 0; i < maxPrograms; i++) { PrgArray[i] = nullptr; }

	findPrograms();
}

// This is the destructor and will clean up anything that was allocated while running
PrgMgmt::~PrgMgmt()
{
	// Let's release any devices that are assigned
	for (int i = 0; i < numPrograms; i++)
	{
		if (PrgArray[i] != nullptr)
		{
			prgParent* p = PrgArray[i];
			p->WrapUp();

			delete p;
			PrgArray[i] = nullptr;
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

// This function will check to see if there are any programs to load into memory
int PrgMgmt::findPrograms()
{
	const char* path = "/etc/IBIS/conf.d/";
	struct dirent* entry;
	DIR* dir = opendir(path);
	if (dir == NULL)
	{
		std::cout << "No programs defined" << std::endl;
		return -1;
	}

	while ((entry = readdir(dir)) != NULL)
		if (std::strstr(entry->d_name,".xml")!=NULL)
			ReadStateMachine(entry->d_name);

	closedir(dir);

	return 0;
}

// This function reads in the State Machine information as needed
void PrgMgmt::ReadStateMachine(const char *fileName)
{
	LEDStatesMachine lState;
	prgLED* l1;

	std::string fName = "/etc/IBIS/conf.d/";
	fName.append(fileName);
//#ifdef DBGMOD
	//std::cout << "Reading:" << fName << std::endl;
//#endif

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fName.c_str());
	pugi::xml_node prg = doc.child("Program");

	ProgramName = doc.child("Program").attribute("name").value();
#ifdef DBGMOD
	std::cout << "Program Name:" << ProgramName << std::endl;
#endif
	DeviceType dt = GetDevType(prg.attribute("type").value());

	switch (dt)
	{
	case DeviceType::LED:
		prgParent* p = new prgLED();
		PrgArray[numPrograms] = p;
		p->setValues(ProgramName.c_str(), (int)dt);
		pugi::xml_node states = prg.child("States");

		for (pugi::xml_node state = states.first_child(); state; state = state.next_sibling())
		{
#ifdef DBGMOD
			for (pugi::xml_attribute attr = state.first_attribute(); attr; attr = attr.next_attribute())
			{
				std::cout << " " << attr.name() << "=" << attr.value();
			}
			std::cout << std::endl;
#endif
			// The common values
			lState.stateNum = state.attribute("id").as_int();
			lState.nextState = state.attribute("nextID").as_int();
			lState.name = state.attribute("name").value();
			lState.cmdCode = state.attribute("code").value()[0];
			// Specific to this device
			lState.delaySecs = state.attribute("delaySeconds").as_int();
			lState.delayMS = state.attribute("delayMS").as_int();
			lState.pinState = state.attribute("PINState").as_int();
			// Ok now let's add this entry
			l1 = (prgLED*)p;
			l1->prgStates.push_back(lState);
		}
		numPrograms++;
		break;
	}
}

// This function reads in the State Machine information as needed
prgParent* PrgMgmt::FindMachine(const char* progName)
{
	for (int i = 0; i < numPrograms; i++)
	{
		if (PrgArray[i] != nullptr)
		{
			prgParent* p1 = PrgArray[i];
			if (p1->IsProgram(progName)) return p1;
		}
	}
	return NULL;
}
