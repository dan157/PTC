#include "devParent.h"

// This structure is just used to convert back and forth between the device type and name
static TypeConv Convert_t1[] = {
	{ "LED", DeviceType::LED },
	{ NULL,  DeviceType::Unknown }
};

// This function is global for all instances of the devmgt class
DeviceType GetDevType(const char* typeName)
{
	std::string tName(typeName);
	for (int i = 0; Convert_t1[i].name != NULL; i++)
	{
		if (std::string(Convert_t1[i].name).compare(tName) == 0) return (Convert_t1[i].value);
	}
	return (DeviceType::Unknown);
}

// This function is global for all instances of the devmgt class
const char* GetDevType(DeviceType typeVal)
{
	for (int i = 0; Convert_t1[i].name != NULL; i++)
	{
		if (Convert_t1[i].value == typeVal) return (Convert_t1[i].name);
	}
	return (NULL);
}



// This function will check to see if the requested elapsed time has exceeded.  If it has, then it will return TRUE, otherwise FALSE
bool devParent::chkDelay(unsigned int chkSec, time_t chkMS)
{
	time_t chkVal;
	if (clock_gettime(clk_id, &timeNow) != 0)
	{
		// Maybe we should add more error handling later on
		return true;
	}
	// First check to see if the seconds has been reached
	if ((timeNow.tv_sec - timeStart.tv_sec) == chkSec)
	{
		// If so then let's check the milliseconds, right now this code is not expected
		// to be able to handle nanoseconds, so let's convert the test to ms
		// MS = 1,000 per second
		// NS = 1,000,000,000 per second
		chkVal = chkMS * 1000000;
		if ((timeNow.tv_nsec - timeStart.tv_nsec) >= chkVal) return true;
	}
	return ((timeNow.tv_sec - timeStart.tv_sec) > chkSec);
}
