/*
 *
 * Author:     Jeang Hasang
 * Created:    2012-8-6
 *
 * recognize object by color and rabeling
 * get wide view by panorama cam
 * read map by sterio cam
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>

#include <pthread.h>

#include <pxa_lib.h>
#include <pxa_camera_zl.h>

#include "panorama.h"
#include "labeling.h"



//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct pxa_camera{
	int camhandle;
	int status;
	int mode;
	int sensor;
	int ref_count;

	// Video Buffer
	int width;
	int height;
	enum pxavid_format format;
};

//function for network
int connectToClient() {
	struct sockaddr_in server;
	socklen_t addrlen;
	int request_sock;
	int new_sock;

	request_sock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset((void*) &server, 0, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons((u_short)10000);
	addrlen=sizeof(server);
	bind(request_sock, (struct sockaddr*)&server,sizeof(server));
	listen(request_sock,SOMAXCONN);

	new_sock = accept(request_sock, (struct sockaddr*) &server, &addrlen);
	while(new_sock == -1) {
//		fprintf(log, "_accept error\n_");
		new_sock = accept(request_sock, (struct sockaddr*) &server, &addrlen);
	}

	close(request_sock);

	return new_sock;
}

//network vals
int new_sock;
int rsize;
int boundary[30];
int recBoundary[30];
int heights[30];
int recHeights[30];
int cordinate[30];

void *netReadThreadFunc(void)
{
		printf("_netReadThreadFuncStart_\n");
		rsize = read(new_sock, recBoundary, sizeof(recBoundary));
		rsize = read(new_sock, recHeights, sizeof(recHeights));
		printf("_netReadThreadFuncStart %d_\n", rsize);
}

int mapread(int inX[], int inY[])
{
	sleep(5);

	//backboard fd
	int backboard;
	//cam and image value
	int camhandle;
	int camhandle_overlay2;
	struct pxa_video_buf* vidbuf;
	struct pxacam_setting camset;
	struct pxa_video_buf vidbuf_overlay2;
	//struct for panorama cam
	struct combinedImg cImg;
	

	//vals
	int size;
	int i, command;
	int timecount;
	//project vals
	int* vProjection;

	//thread vals
	pthread_t netReadThread;
	int thread_id;

	//connect whit backboard
	backboard=open("/dev/ttyS2", O_RDWR | O_NOCTTY);
	if( backboard < 0 ) {
		printf("connect backboard error.\n");
		return -1;
	}

	//open cam
	camhandle = camera_open(NULL,0);
	ASSERT(camhandle);
	//init camsetting
	memset(&camset,0,sizeof(camset));
	camset.mode = CAM_MODE_VIDEO;
	camset.format = pxavid_ycbcr422;
	camset.width = 320;
	camset.height = 240;
	//bind setting
	camera_config(camhandle, &camset);
	//start cam
	camera_start(camhandle);

	//get total pixel size
	size = camset.width * camset.height;
	
	//i don`t know why but it can`t skip
	camhandle_overlay2 = overlay2_open(NULL, camset.format, NULL, camset.width, camset.height, 0 , 0);
	overlay2_getbuf(camhandle_overlay2, &vidbuf_overlay2);

	//init cimg struct and val
	initCombinedImg (&cImg, &camset);
	timecount = 0;

	//malloc project vals
	vProjection = (int*)malloc(sizeof(int)*cImg.width);

	pre();

	//netReadThread start
	pthread_create(&netReadThread, NULL, netReadThreadFunc, (void*)NULL);

	//infi loop
	while(1){
		//get frame from cam
		vidbuf = camera_get_frame(camhandle);

		//copy frame to var
		memcpy(vidbuf_overlay2.ycbcr.y,vidbuf->ycbcr.y,size);
		memcpy(vidbuf_overlay2.ycbcr.cb,vidbuf->ycbcr.cb,size/2);
		memcpy(vidbuf_overlay2.ycbcr.cr,vidbuf->ycbcr.cr,size/2);
		
		if( timecount < 81 ) {
			timecount++;

			//when start watch front
			if( timecount == 50 ) {
				command = 26;
				write(backboard ,&command ,1);		
			}
			//put front img to struct and trun head
			else if ( timecount == 55 )
			{
				saveHalfImg (&cImg, 1, 0, &vidbuf_overlay2);
				command = 23;
				write(backboard,&command,1);
			}
			//put side img to struct and watch front
			else if ( timecount == 70)
			{
				saveHalfImg (&cImg, 0, 1, &vidbuf_overlay2);
				command = 32;
				write(backboard,&command,1);
			}
			//print img to lcd
			else if ( timecount == 80 ) {
				//vertical projection
				verticalProject(&cImg, vProjection);
				setBoundary(&cImg, vProjection, boundary);
				getYVals(&cImg, vProjection, boundary, heights);
				drawProject(&cImg, vProjection);
				drawBoundary(&cImg, boundary);

				viweCombinedImg(&cImg, &vidbuf_overlay2);

				stereoX(boundary, recBoundary, heights, recHeights, cordinate);

				for(i = 0 ;i < 30; i+=2) {
					inX[i/2] = cordinate[i];
					inY[i/2] = cordinate[i+1];
				}

				sleep(3);
			}
		}

		//not only print to lcd
		//it also clean buffer!!!!!!!!!!!!
		//so it must not skip!!!!!!!!!!!!!
		camera_release_frame(camhandle,vidbuf);

		if( timecount >= 80 ) {
			break;
		}

		fprintf(log, "_end_\n");
	}//end while
	
	//free project vals
	free(vProjection);

	//free cimg struct
	freeCombinedImg(&cImg);

	//end backboard
	close(backboard);

	//close socket
	close(new_sock);

	//end cam
	camera_stop(camhandle);
	camera_close(camhandle);
	overlay2_close(camhandle_overlay2);

	return 0;
}

