// James' experimenting app
#include <nuttx/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <systemlib/err.h>
#include <unistd.h>
#include <drivers/drv_hrt.h>
#include <math.h>
#include <nuttx/can.h>
#include "can.h"

__EXPORT int jamesJunk_main(int argc, char *argv[]);

int jamesJunk_main(int argc, char *argv[])
{
	printf("Starting canReader\n");
	// CAN variables:
	struct can_msg_s receivedMessage, sendMessage;
	size_t msgsize;
	ssize_t nbytes;
	int fd;
//	int msgdlc;
//	uint8_t msgdata;
//	uint16_t msgid;
	msgsize = CAN_MSGLEN(8);
	int ret;
//	int errval = 0;

	// Init CAN interface here for starters
	printf("jamesJunk_main: Initializing external CAN device\n");
	struct can_dev_s *can;
	// NOTE here we initialise different cans for different px4's. 1 for pixhawk, 2 for px4.
	can = stm32_caninitialize(1);

	if(can == NULL){
		printf("failed to init can\n");
		return -1;
	}

	ret = can_register("/dev/can0", can);
	message("ret is: %d \n",ret);
	if (ret < 0) {
		message("ERROR: can_register failed: %d\n", ret);
		return ret;
	}
	else
	{
		printf("success\n");
	}
	usleep(1000000); // Sleep a little, then it should open!!
	// We are now going to open the CAN device for reading.
	printf("jamesJunk_main: Hardware initialised. Opening the CAN device\n");
	fd = open(CONFIG_EXAMPLES_CAN_DEVPATH, 3);

	if (fd < 0)
	{
		printf("jamesJunk_main: open %s failed: %d\n",
		CONFIG_EXAMPLES_CAN_DEVPATH, errno);
	}
	else
		printf("device open!\n");
	// Seems to work upto here
	//
	// Now I'm just going to read a single message and print it to terminal...
	//
	// Try and send a message to minimon
	msgflush();
	sendMessage.cm_data[0] = 0; // mode, 0 means rpm is controlled
	sendMessage.cm_data[1] = 0; // stop flag
	sendMessage.cm_hdr.ch_id = 8;
	sendMessage.cm_hdr.ch_rtr = false;
	sendMessage.cm_hdr.ch_dlc = 8;
	sendMessage.cm_data[2] = 0;
	sendMessage.cm_data[3] = 0;
	sendMessage.cm_data[4] = 0;
	sendMessage.cm_data[5] = 0;
	sendMessage.cm_data[6] = 0;
	sendMessage.cm_data[7] = 0;
	sendMessage.cm_data[7] = 	sendMessage.cm_data[7] + 1;

	// while (true)
	// {
	// usleep(1000000);
	// message("Before send\n");
	// nbytes = write(fd,&sendMessage,msgsize);
	//
	// if (nbytes != msgsize)
	// {
	// message("ERROR: write(%d) returned %d\n", msgsize, nbytes);
	// errval = 3;
	// }
	// else
	// message("sent CAN msg\n");
	//
	// msgflush();
	// }
	//
	// message("After send\n");
	msgflush();
	message("reading message\n");
	nbytes = read(fd, &receivedMessage, msgsize);
	int a;
	a = nbytes;
	a = a + 1;
	message("read");
	message(" ID: %4d DLC: %d\n", receivedMessage.cm_hdr.ch_id, receivedMessage.cm_hdr.ch_dlc);
	return OK;
}
