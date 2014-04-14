/*
*
* Author:     Jeang Hasang
* Created:    2012-7-28
*
* stoper 
*
*/

#include <stdio.h>
#include <signal.h>

int fd;
struct sigaction act;
void sighandler(int signo)
{
	close(fd);
	exit(0);
}

int main(int argc, char* argv[])
{
	int command;

	//connect whit backboard
	fd=open("/dev/ttyS2", O_RDWR | O_NOCTTY);
	if( fd < 0 ) {
		printf("connect backboard error");
		return -1;
	}

	command = 26;
	write(fd,&command,1);

	return 0;
}
