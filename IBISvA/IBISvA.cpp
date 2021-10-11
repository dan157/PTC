#include "devmgt.h"
#include "PrgMgmt.h"
#include <csignal>
#include <wiringPi.h>


static devmgt* dev;
static PrgMgmt* prg;

// This signal handler is used to make sure that everything is properly wrapped up when a user wants to break out of the program
void signalHandler(int signum) 
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";

	if (dev!=NULL) delete dev;
	dev = NULL;

	exit(signum);
}

// This process will initiate all functions needed to perform the tasks of this app.
int main(void)
{
	if (wiringPiSetup() == -1)
	{
		printf("setup wiringPi failed!\n");
		return -1;
	}

	signal(SIGINT, signalHandler);

#if 1
	prg = new PrgMgmt();
	dev = new devmgt();
	dev->findDevices(prg);

	while (true)
	{
		dev->HandleStates();
	}

	delete dev;
	dev = NULL;

	delete prg;
	prg = NULL;
#else
	::digitalWrite(1, LOW);
	::pinMode(1, 1);
	::digitalWrite(1, HIGH);
#endif

	return 0;
}
