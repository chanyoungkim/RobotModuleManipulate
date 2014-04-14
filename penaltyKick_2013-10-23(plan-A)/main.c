#include "main.h"

int main(int argc, char* argv[]) {
	// Initialization
	initDevices();
	initNetwork(); // network and role

	usleep(10000000); // delay 1sec

	// Strart linetracing
	int thr_id[4];
	pthread_t p_threads[4];
	
	
	thr_id[0] = pthread_create(&p_threads[0], NULL, videoFrame, (void *)NULL);     
	if (thr_id[0] < 0)     
	{    
		perror("videoFrame create error : ");  
		exit(0);    
	}
	
	usleep(1000000); // delay 1sec	
	
	thr_id[1] = pthread_create(&p_threads[1], NULL, doMission, (void *)NULL);     
	if (thr_id[1] < 0) {
		perror("doMission thread create error : ");  
		exit(0);    
	}

	int status;
	pthread_join(p_threads[0], (void **)&status);
	pthread_join(p_threads[1], (void **)&status);
	
	return 0;
}  


