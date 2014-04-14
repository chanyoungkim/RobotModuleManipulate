/*
*
* Author:     Kobot
* Created:    2012-8-25
*
* Sample code interface header for PXA Linux release. All interfaces
* introduced in this file is not multiple thread safe unless it's 
* specified explicitly. 
*
*/

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pxa_lib.h>
#include <pxa_camera_zl.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <netdb.h>
#include <pthread.h>


#include "variables.h"
#include "map_read.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct pxa_camera{
	int handle;
	int status;
	int mode;
	int sensor;
	int ref_count;

	// Video Buffer
	int width; 
	int height;
	enum    pxavid_format format;
};



int main(int argc, char* argv[])
{
	//file
	FILE* log;

	int startIndex=0;
	int i;	

	int obX[15];
	int obY[15];

	//mapread(obX, obY);
	//acopy(obX, obY);

	/*for(i = 0 ; i < 15 ; i++){
		printf("!!!!!!!!!!![%d %d]", obX[i], obY[i]);
	}*/

	pthread_t stopThread, camThread,moveThread;
	void* thReturn;


	//log file open
	log = fopen("log.txt", "w");
	if( log == NULL ) {
		printf("log file open error\n");
		return -1;
	}
	else {
		printf("log file opened\n");
		fprintf(log,"log file opened\n");	
	}

	sleep(3);
	pthread_create(&stopThread, NULL, checkdist, (void*)NULL);
	pthread_create(&camThread, NULL, findObj, (void*)NULL);
	pthread_create(&moveThread, NULL, backBoard, (void*)NULL);
	

	pthread_join(stopThread, &thReturn);
	pthread_join(camThread, &thReturn);	
	pthread_join(moveThread, &thReturn); 

	return 0;
}
