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
//#include "moduls/vectorize.h"
#include "moduls/vectorize_2013_07_08.h"


#define BAUDRATE B4800
#define MODEDEVICE "/dev/ttyS2"

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
	
	//get frame from cam	first
	vidbuf = camera_get_frame(handle);
	
	usleep(10000000);	// delay 10sec 어서 코드 뽑아
	

	//init move
	command = 21;			// 시작할 때 무조건 앞으로 전진한다
	write(uart_fd,&command,1);
	
	int notFoundCnt = 0;
	while(1){
		int count = 0;		// 회전 횟수
		int direction = -1;	// 회전 방향 0이면 왼쪽, 1이면 오른쪽

		//copy frame to var
		memcpy(vidbuf_overlay.ycbcr.y,vidbuf->ycbcr.y,len);
		memcpy(vidbuf_overlay.ycbcr.cb,vidbuf->ycbcr.cb,len/2);
		memcpy(vidbuf_overlay.ycbcr.cr,vidbuf->ycbcr.cr,len/2);
		count = vectorize(&vidbuf_overlay, &direction);

		if(count == -1){
			notFoundCnt++;
		}	
		else
			notFoundCnt = 0;
		
		if(notFoundCnt > 2){
			direction = 0;
			count = 1;
			notFoundCnt--;
		}	

 		if(direction == 0){
			command = 28;	// 걸으면서 왼쪽 회전
		}
		else if(direction == 1){
			command = 30;	// 걸으면서 오른쪽 회전
		}
		else
			command = 5;	// 소리
		
	 	int i=0;
		for(i=0; i<count; i++){	// 회전의 세기만큼 명령 반복수행
			//send to backboard
			write(uart_fd,&command,1);
			usleep(100000);	// delay 0.5sec
		}


		//not only print to lcd
		//it also clean buffer!!!!!!!!!!!!
		//so it must not skip!!!!!!!!!!!!!
		camera_release_frame(handle,vidbuf);
		
		//get frame from cam
		vidbuf = camera_get_frame(handle);
		
	}	
	
	//end cam
	camera_stop(handle);
	camera_close(handle);
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

