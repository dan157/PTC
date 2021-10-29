/*! \file IBISvA.cpp
 * \brief This is the main control loop for the system
 */
/*! \mainpage IBIS System Version A
 *
 * \section intro_sec Introduction
 * 
 * This is the IBIS System Version A, the intentions of this system is to simplify controlling various devices with the goal of easy or no programming skills required to do so.
 * 
 * The GitHub location for the original code is https://github.com/dan157/PTC and the YouTube channel used to watch the videos to explain various features of this system can be found searching for the channel called "Pacific Tech Center".
 * 
 * \author Dan Woods
 * \version 0.1
 * \copyright MIT (c) 2021 (See License file located in the directory for more details)
 * \warning There are no expectations of warranty or liability of any kind
 * 
 * \section install_sec Installation
 * 
 * \subsection step1 Step 1: Clone the github page https://github.com/dan157/PTC
 * 
 * \subsection step2 Step 2: Compile the program by typing "make" from the PTC/IBISvA directory
 * 
 * \subsection step3 Step 3: Read the manual, currently located in the PTC/IBISvA/Examples/Chapter1, the UserManual.pdf or UserManual.docx should exists if the program was cloned fully
 * 
 * \subsection step4 Step 4: Either copy the system.xml file from the same directory, or create a new one and place it into the /etc/IBIS/conf/ directory
 * 
 * \subsection step5 Step 5: Either copy the example *.xml files (all except for the system.xml file) to the /etc/IBIS/conf.d/ directory or create your own State Machine program XML files and place them here
 * 
 * \subsection step6 Step 6: Run the IBIS program "./IBISvA" or redirect to /dev/null if you don't want to see the debug code "./IBISvA > /dev/null"
 * 
 */
#include "devmgt.h"
#include "PrgMgmt.h"
#include <csignal>
#include <wiringPi.h>


static devmgt* dev; /*!< Static Pointer that tracks all devices being managed by the system  */
static PrgMgmt* prg; /*!< Static Pointer that tracks all State Machine Program information for all devices */

//! This signal handler is used to make sure that everything is properly wrapped up when a user wants to break out of the program
/*!
 * \param signum This is the signal type that will be handled by this function.  SIGINT is used to capture the Ctrl-C keyboard command allowing us to perform a normal shutdown prior to exiting
 */
void signalHandler(int signum) 
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";

	if (dev!=NULL) delete dev;
	dev = NULL;

	exit(signum);
}

//! This process will initiate all functions needed to perform the tasks of this app.
int main(void)
{
	if (wiringPiSetup() == -1)
	{
		printf("setup wiringPi failed!\n");
		return -1;
	}

	signal(SIGINT, signalHandler);

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

	return 0;
}
