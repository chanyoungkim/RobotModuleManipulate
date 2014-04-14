/* 2013 FIRA HuroCup 
 *
 *	Title: Penalty Kick
 *
 *	Author:	Kobot (Kookmin Univ. CS)
 */

#include "main.h"




// 백보드, 3축 센서
int fdBackBoard;
int fdThreeAxis;

// 카메라, 오버레이
int fdCamera;
int fdOverlay2;

struct pxa_video_buf* vidbuf;
struct pxa_video_buf vidbuf_overlay;
int len_vidbuf;

VideoCopy bufCopy;

// Fuction Prototypes
void initDevices(void); // Initialize Camera and Sensors
int openSerial(void);
void endProgram(void);
void refreshImage(void);
int writeCommand(int cmd); // Write command to backboard uart port
void findObjectColor(int interval);
void screenFoundObject();
void setBoundary(HSV hsv, int j, int i);




void* videoFrame(void){
	int cnt = 0;
	while(1){
		vidbuf = camera_get_frame(fdCamera);
	
		memcpy(bufCopy.ycbcr.y,vidbuf->ycbcr.y,len_vidbuf);
		memcpy(bufCopy.ycbcr.cb,vidbuf->ycbcr.cb,len_vidbuf/2);
		memcpy(bufCopy.ycbcr.cr,vidbuf->ycbcr.cr,len_vidbuf/2);
		
		camera_release_frame(fdCamera,vidbuf);

	}
}

// start of function main()
int main(int argc, char* argv[]) {

	// Initialization
	initDevices();
	usleep(3000000); // delay 3sec
	system("/root/test_app/cam_config/awb_ae_off/bg_setoff");
	usleep(3000000);
	
	int thr_id[4];
	pthread_t p_threads[4];
	
	
	thr_id[0] = pthread_create(&p_threads[0], NULL, videoFrame, (void *)NULL);     
	if (thr_id[0] < 0)     
	{    
		perror("videoFrame create error : ");  
		exit(0);    
	}
	
	usleep(2000000); // delay 3sec
/*
	thr_id[2] = pthread_create(&p_threads[2], NULL, receiveMsg, (void *)NULL);     
	if (thr_id[1] < 0) {
		perror("receive thread create error : ");  
		exit(0);    
	}
*/	

	
	  findObjectColor(1);
	
	
	int status;
	pthread_join(p_threads[0], (void **)&status);


	//screenFoundObject();
	
//	endProgram();
	return 0;
}

int obj=0;
float maxh=0, minh=360, maxs=0, mins=100, maxv=0, minv=100;
int maxX =0, minX=320, maxY=0, minY=240;
// Print HSV Color Matrix
void findObjectColor(int interval) {
	
	int first_Flag = 1;
	ColorBoundary color_B;
	int color_Type = 0;
	while(1){
	    HSV hsv;
	    int i, j;
	    int count=0;		
	    int index=0;
    //	vidbuf = camera_get_frame(fdCamera);
    //	usleep(3000000); // delay 3sec
    //	camera_release_frame(fdCamera, vidbuf);
    //	vidbuf = camera_get_frame(fdCamera);

	    
	    
	    if(first_Flag == 1)
	    {
		first_Flag = 0;
			printf("Input first boundary Type\n");
			printf("1 : blue, 2 : green, 3:red, 4:yellow\n");
			scanf("%d", &color_Type);		
			if(color_Type == 1)
			{
				color_B.hmin = 190;
				color_B.hmax = 250;
				color_B.smin = 40;
				color_B.smax = 100;
				color_B.vmin = 28;
				color_B.vmax = 100;
			}
			else if(color_Type == 2)
			{
				color_B.hmin = 120;
				color_B.hmax = 168;
				color_B.smin = 47;
				color_B.smax = 100;
				color_B.vmin = 18;
				color_B.vmax = 100;
			}
			else if(color_Type == 3)	
			{
				color_B.hmin = 345;
				color_B.hmax = 15;
				color_B.smin = 45;
				color_B.smax = 100;
				color_B.vmin = 40;
				color_B.vmax = 100;
			}
			else if(color_Type == 4)	
			{
				color_B.hmin = 43;
				color_B.hmax = 76;
				color_B.smin = 40;
				color_B.smax = 100;
				color_B.vmin = 40;
				color_B.vmax = 100;
			}
	    }
	    else
	    {
		char change_Flag;
	      
		printf("value change? y or n\n");
		scanf("%s", &change_Flag);
		
		if(change_Flag == 'y')
		{
		    int whichOne;
		    printf("h : 1, s ; 2, v : 3\ninput : ");\
		    scanf("%d", &whichOne);
		    
		    if(whichOne == 1)
		    {
			printf("H min : ");
			scanf("%d", &(color_B.hmin));
			printf("H max : ");
			scanf("%d", &(color_B.hmax));
		    }
		    else if(whichOne == 2)
		    {
			printf("S min : ");
			scanf("%d", &(color_B.smin));
			printf("S max : ");
			scanf("%d", &(color_B.smax));
		    }
		    else if(whichOne == 3)
		    {
			printf("V min : ");
			scanf("%d", &(color_B.vmin));
			printf("V max : ");
			scanf("%d", &(color_B.vmax));
		    }
		}
		
	    }
	    
	    printf("hmin : %d, hmax : %d\n", color_B.hmin, color_B.hmax); 
	    printf("smin : %d, smax : %d\n", color_B.smin, color_B.smax);
	    printf("vmin : %d, vmax : %d\n", color_B.vmin, color_B.vmax);
	    
	    VideoCopy buf2 = bufCopy;
	    
	    //copy frame to var
	    memcpy(vidbuf_overlay.ycbcr.y, buf2.ycbcr.y,len_vidbuf);
	    memcpy(vidbuf_overlay.ycbcr.cb, buf2.ycbcr.cb,len_vidbuf/2);
	    memcpy(vidbuf_overlay.ycbcr.cr, buf2.ycbcr.cr,len_vidbuf/2);
	    
	    for (i = MAX_Y - (interval / 2); i >= 0; i -= interval) {
		    for (j = MAX_X - (interval / 2); j >= 0; j -= interval) {	
			    hsv = getHSV(&buf2, j, i);
			    index = 320*i + j;			
			    
			    if(isColor(hsv, color_B))
			    {	
			      count++;
			      if(count==1)
				printf("in func\n");
			      
			    obj = 1;			
			    setBoundary(hsv, j, i);
			    
				    vidbuf_overlay.ycbcr.y[index] = 100;	
				    vidbuf_overlay.ycbcr.cb[index/2] = 200;
				    vidbuf_overlay.ycbcr.cr[index/2] = 200;
			    }
		    }
	    }
	    if(obj == 1){
		    printf("Found Milk\n");
		    printf("(minh, maxh, mins, maxs, minv, maxv : (%f, %f, %f, %f, %f, %f)\n", minh, maxh, mins, maxs, minv, maxv);

	    }
	    char tmptmp[100];

	    printf("insert something : ");
	    scanf("%s", tmptmp);
	  
	}
	
}

void screenFoundObject()
{
	int i=0, j=0, index;
	for(i = minX; i<maxX; i++){
		for(j = minY; j<maxY; j++){
			index = i*320+j;
			vidbuf = camera_get_frame(fdCamera);
			//copy frame to var
			vidbuf->ycbcr.y[index] = 100;
			vidbuf->ycbcr.cb[index/2] = 20;
			vidbuf->ycbcr.cr[index/2] = 200;			
		}
	}
}
void setBoundary(HSV hsv, int j, int i)
{
	if(maxh<hsv.h)
		maxh= hsv.h;
	if(minh>hsv.h)
		minh = hsv.h;

	if(maxs<hsv.s)
		maxs = hsv.s;
	if(minh>hsv.s)
		mins = hsv.s;

	if(maxv<hsv.v)
		maxv = hsv.v;
	if(minh>hsv.v)
		minv = hsv.v;

	if(maxX < i)
		maxX = i;
	if(minX > i)
		minX = i;

	if(maxY < j)
		maxY = j;
	if(minY > j)
		minY = j;
}

// Caculate direction and produce command value
void refreshImage(void) {
	//get frame from cam
	vidbuf = camera_get_frame(fdCamera);
	usleep(3000000); // delay 3sec
	camera_release_frame(fdCamera, vidbuf);
	vidbuf = camera_get_frame(fdCamera);

	//copy frame to var
	memcpy(vidbuf_overlay.ycbcr.y, vidbuf->ycbcr.y,len_vidbuf);
	memcpy(vidbuf_overlay.ycbcr.cb, vidbuf->ycbcr.cb,len_vidbuf/2);
	memcpy(vidbuf_overlay.ycbcr.cr, vidbuf->ycbcr.cr,len_vidbuf/2);
	
	memcpy(bufCopy.ycbcr.y,vidbuf->ycbcr.y,len_vidbuf);
    memcpy(bufCopy.ycbcr.cb,vidbuf->ycbcr.cb,len_vidbuf/2);
    memcpy(bufCopy.ycbcr.cr,vidbuf->ycbcr.cr,len_vidbuf/2);
   
	// !!essential!!
	// put image to screen & clean buffer 
	camera_release_frame(fdCamera, vidbuf);
}

// Write command to backboard uart port
int writeCommand(int cmd) {
	printf("command: %d\n", cmd);
	write(fdBackBoard, &cmd, 1);
}

// Initialize Camera and Sensors
void initDevices(void) {
	struct pxacam_setting camset;
	
	// Backboard uart init
	fdBackBoard = openSerial();

	// 3-axis sensor init 
	// fdThreeAxis = open("/dev/MMA_ADC", O_RDONLY);
	// ASSERT(fdThreeAxis);

	// ioctl(fdThreeAxis,MMA_VIN_ON, 0);
	// ioctl(fdThreeAxis,MMA_SLEEP_MODE_ON, 0);
	// ioctl(fdThreeAxis,MMA_SENS_60G, 0);

	// Camera init
	fdCamera = camera_open(NULL,0);
	ASSERT(fdCamera);

	memset(&camset,0,sizeof(camset));
	camset.mode = CAM_MODE_VIDEO;
	camset.format = pxavid_ycbcr422;

	camset.width = 320;
	camset.height = 240;

	camera_config(fdCamera,&camset);
	camera_start(fdCamera);

	fdOverlay2 = overlay2_open(NULL,pxavid_ycbcr422,NULL, 320, 240, 0 , 0);

	overlay2_getbuf(fdOverlay2, &vidbuf_overlay);
	len_vidbuf = vidbuf_overlay.width * vidbuf_overlay.height;

	// init finish
	printf("Initializing Device Finished\n");	
}

void endProgram(void) {
	camera_stop(fdCamera);
	camera_close(fdCamera);
}

int openSerial(void){
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
