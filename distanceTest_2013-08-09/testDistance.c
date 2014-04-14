/*
 *   MINI ROBOT 
 *
 *   robot uart control
 *  
 *   <newboder@hybus.net>
 *
 *	 Modifier	: Kim Moon Ki
 *	 Date		: 2013-07-08 
 *	 Test Name	: Line tracing solo
 */

 
 
#include <stdio.h>
//#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include <time.h>
//#include <sys/ioctl.h>
//#include <sys/time.h>
#include <sys/types.h>

// 원본
#include <stdlib.h>
#include <termios.h>
#include <sys/signal.h>
#include <string.h>
#include <pthread.h> 
#include <unistd.h> 

// 카메라 모듈 관련 헤더
#include <pxa_lib.h>
#include <pxa_camera_zl.h>

// 벡터라이즈 헤더
#include "moduls/stereo.h"
//#include "moduls/vectorize_2013_07_08.h"



#define BAUDRATE B4800
#define MODEDEVICE "/dev/ttyS2"
#define max(a,b) (((a)>(b))? (a):(b))
#define min(a,b) (((a)<(b))? (a):(b))
#define MAX_3(a, b, c)	( ((a)>(b)) ? ( (((a)>(c)) ? (a) : (c)) ) : ( (((b)>(c)) ? (b) : (c)) ) )
#define MIN_3(a, b, c)	( ((a)>(b)) ? ( (((b)>(c)) ? (c) : (b)) ) : ( (((a)>(c)) ? (c) : (a)) ) )


int uart_fd;


int open_serial(void)
{
    char fd_serial[20];
    int fd;
    struct termios oldtio, newtio;

    strcpy(fd_serial, MODEDEVICE); //FFUART
    
    fd = open(fd_serial, O_RDWR | O_NOCTTY );
    if (fd <0) {
        printf("Serial %s  Device Err\n", fd_serial );
        exit(1);
    }
	printf("robot uart ctrl %s\n", MODEDEVICE);
    
    tcgetattr(fd,&oldtio); /* save current port settings */
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 8 chars received */
    
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    
    return fd;
}

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
// if s == 0, then h = -1 (undefined)
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	min = MIN_3(r, g, b);
	max = MAX_3(r, g, b);

	//printf("copy r g b is : %.0f %.0f %.0f\nmin max is : %f %f\n\n", r, g, b, min, max);
	*v = max; // v
	
	delta = max - min;

	if( max != 0 )
		*s = delta / max; // s
	else {
		// r = g = b = 0 // s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta; // between yellow & magenta

	else if( g == max )
		*h = 2 + ( b - r ) / delta; // between cyan & yellow

	else
		*h = 4 + ( r - g ) / delta; // between magenta & cyan
	
	*h *= 60; // degrees
	if( *h < 0 )
		*h += 360;
}



void *write_func(void ) 
{    

	// 카메라 오버레이 관련
	int handle;
	int handle_overlay2;
	struct pxa_video_buf* vidbuf;
	struct pxacam_setting camset;
	struct pxa_video_buf vidbuf_overlay;
	
	//vals
	int command;
	int len;

	usleep(1000000);
	//cam init
	handle = camera_open(NULL,0);
	ASSERT(handle);

	memset(&camset,0,sizeof(camset));
	camset.mode = CAM_MODE_VIDEO;
	camset.format = pxavid_ycbcr422;

	camset.width = 320;
	camset.height = 240;

	camera_config(handle,&camset);

	camera_start(handle);

	handle_overlay2 = overlay2_open(NULL,pxavid_ycbcr422,NULL, 320, 240, 0 , 0);

	overlay2_getbuf(handle_overlay2,&vidbuf_overlay);
	len = vidbuf_overlay.width*vidbuf_overlay.height;
	//cam init end

	//usleep(1000000);	// delay 1sec

	int n=0;	int fooCnt=0;
	float maxh = 0, minh = 360, maxs = 0, mins = 1, maxv =0, minv=1;
	while(1){
		
		char s[3];

		//get frame from cam
		vidbuf = camera_get_frame(handle);
		//copy frame to var
		memcpy(vidbuf_overlay.ycbcr.y,vidbuf->ycbcr.y,len);
		memcpy(vidbuf_overlay.ycbcr.cb,vidbuf->ycbcr.cb,len/2);
		memcpy(vidbuf_overlay.ycbcr.cr,vidbuf->ycbcr.cr,len/2);
		int a, b;
		for(a=158; a<162; a++){
			for(b = 118; b<122; b++){
				int index = 320*b + a;		
				
				vidbuf_overlay.ycbcr.y[index] = 80;
				vidbuf_overlay.ycbcr.cb[index/2] = 20;			
				vidbuf_overlay.ycbcr.cr[index/2] = 240;
			}
		}
		
		int x, k;
		int j=0, l=0, m=0;

		if(fooCnt > 20){
		for(k = 125; k > 115 ; k--){
			int bluecnt = 0;				

			for(x = 155; x <  165;x++){
				int index = k*320 + x;
				
				// set ycbcr val
				int y = vidbuf_overlay.ycbcr.y[index];
				int cb = vidbuf_overlay.ycbcr.cb[index/2];
				int cr = vidbuf_overlay.ycbcr.cr[index/2];

				
				int r = min( max( ((y-16))*255.0/219.0 + 1.402*((cr-128)*255.0)/224.0 + 0.5,  0 ) ,  255);
				int g = min( max( ((y-16))*255.0/219.0 - 0.344*((cb-128)*255.0)/224.0 - 0.714*((cr-128)*255.0)/224.0 + 0.5,  0 ) ,  255);
				int b = min( max( ((y-16))*255.0/219.0 + 1.772*((cb-128)*255.0)/224.0 + 0.5, 0 ) ,  255);

//printf("%d %d %d -> %d %d %d\n", y, cb, cr, r, g, b);
		
				float h, s, v;
				
				float h1[600], s1[600], v1[600];
				RGBtoHSV(r, g, b, &h, &s, &v);
				v = v/255.0f;
				printf("%f, %f, %f\n", h,s,v);	
				
				h1[j]=h;
				s1[l]=s;
				v1[m]=v;
		
				int x_cord, y_cord;
			
				
								
//		k = vectorize(&vidbuf_overlay, &direction);
		
//		printf("test %d\n", count);

// 		if(direction == 0){
//			command = 28;	// 걸으면서 왼쪽 회전
//			command = 2;}
//		else if(direction == 1){
//			command = 30;	// 걸으면서 오른쪽 회전
//			command = 8;}
//		else
//			command = 5;	// 소리

//	 	int i=0;
//		for(i=0; i<count; i++){
			//send to backboard
//			write(uart_fd,&command,1);
//		}

		//not only print to lcd
		//it also clean buffer!!!!!!!!!!!!
		//so it must not skip!!!!!!!!!!!!!
		camera_release_frame(handle,vidbuf);

		
	}	
	
	//end cam
	camera_stop(handle);
	camera_close(handle);
	
	//////////////////////////////////////////////////

/*
	char s[2+1];
	int write_buf;

	while(1)     
	{         
		fgets(s, sizeof(s), stdin);
		strchr(s, '\n');
		write_buf = atoi(s);

		if( write_buf != 0 )
		{
			printf("Embedded->Robot [%d]\n", write_buf);
			write(uart_fd, &write_buf, 1 );
		}
	} 
*/
}  

void *read_func(void ) 
{     
    char read_buf;
 
	while(1)     
	{         
        read(uart_fd, &read_buf, 1);

        if( read_buf != 0 )
			printf("Robot->Embedded [%d]\n", read_buf);
	} 
}  

int main(int argc, char* argv[])
{
    int write_buf;
    char read_buf;

    int thr_id1;
    int thr_id2;
    int status;
    pthread_t p_thread[2];
	
    uart_fd = open_serial();

	thr_id1 = pthread_create(&p_thread[0], NULL, write_func, (void *)NULL);     
	if (thr_id1 < 0)     
	{    
		perror("thread1 create error : ");  
		exit(0);    
	}      
/*
	thr_id2 = pthread_create(&p_thread[1], NULL, read_func, (void *)NULL);  
	if (thr_id2 < 0)  
	{    
		perror("thread2 create error : ");  
		exit(0);  
	}
*/

	

	pthread_join(p_thread[0], (void **)&status);   
//	pthread_join(p_thread[1], (void **)&status);     
	return 0;
}  



