//	James Reeves 07/10/2014
//
//	This Daemon manages the state of lockWing. It reads the appropriate switch on the remote,
//  and sets the value of a variable in a uORB that all other processes subscribe to. This individual controllers
//  (fixed wing and lock rotor) are aware of this and respond by them selves to this "global" state variable.
//
//  This daemon is responsible for handling all transition related communications (CAN messages), with the pods too.
//  It has its own logic and will perform a set of checks before executing any vehicle state change.
//
//	It is done this way so as to keep the controllers and other scripts relatively clean. The only modification
//  necessary to the controllers' code is a state check before they publish anything. Thus, all controllers are running
//  simultaneously, but can only publish data if the vehicle is in "their" state. This is to avoid initialisation problems,
//  start up delays, and so on.
//
//  States:
//		0 - Rotor craft
//		1 - Locked Wing (fixed wing)
//		5 - Transition from rotor craft to fixed wing
//		6 - Transition from fixed wing to rotor craft.
//		9 - Boot up state.
//

#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <systemlib/systemlib.h>
#include <systemlib/err.h>
#include <unistd.h>
#include <drivers/drv_hrt.h>
#include <math.h>
// uORB includes
#include <uORB/uORB.h>
#include <uORB/topics/vehicle_transition_state.h>
#include <uORB/topics/airspeed.h>



// Exports and prototypes
__EXPORT int transition_state_controller_main(int argc,char *argv[]);

int transition_state_controller_thread_main(int argc,char *argv[]);

static void usage(const char *reason);


//
// Global variables
//
static bool thread_running = false;
static bool thread_should_exit = false;
static int daemon_task;
static int vehicle_state = 9;

//
// Functions
//

// Print some usage information.
static void usage(const char *reason)
{
	if (reason)
		warnx("%s\n",reason);
	errx(1,"usage: transition_state_controller {start|stop}\n\n");
}

// The main thread of the app.
int transition_state_controller_thread_main(int argc,char *argv[])
{
	warnx("[transition_state_controller] starting...\n");
	thread_running = true;

	// Declare thread variables.
	struct vehicle_transition_state_s transitionState;
	memset(&transitionState,0,sizeof(transitionState));
	// Add a minimum height and airspeed condition later.

	// Advertise that this app is publishing the state variable
	orb_advert_t vehicle_state_pub = orb_advertise(ORB_ID(vehicle_transition_state),&transitionState);

	// Subscribe to uORBS. At present we don't subscribe to anything.



	// This is a crap line because it was moaning about variable not being used. This was causing an error for some reason.
	vehicle_state = 1;

	while (!thread_should_exit){
		//
		// We're just going to publish the vehicle_state variable.
		//

		// 1. set the local state variable.
		transitionState.vehicle_state = vehicle_state;

		// 2. Publish it for all to see. First check if updated.
		orb_publish(ORB_ID(vehicle_transition_state), vehicle_state_pub, &transitionState);

		// Run at more or less 5 Hz. This should be fine.
		usleep(200000) ;
	}

	return 0;
}

// This function serves only to start and stop the app.
int transition_state_controller_main(int argc,char *argv[])
{
	if (argc < 1)
		usage("Missing Command");

	if(!strcmp(argv[1],"start")) {

		if (thread_running) {
			warnx("Daemon already running\n");
			exit(0);
		}

		thread_should_exit = false;
		daemon_task = task_spawn_cmd("Transition_state_controller",
										SCHED_RR,
										200,
										2048,
										transition_state_controller_thread_main,
										(argv) ? (const char **)&argv[2] : (const char **)NULL);
		exit(0);
	}


	if(!strcmp(argv[1],"stop")) {
			thread_should_exit = true;
			exit(0);
	}

	usage("unrecognised command");
	exit(1);
}
