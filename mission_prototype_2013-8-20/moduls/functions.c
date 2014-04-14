

#include <pxa_lib.h>
#include <pxa_camera_zl.h>
#include <stdlib.h>
#include <math.h>
#include "angle_recognition.h"
#include "variables.h"

/*int objX[15];
int objY[15];

void acopy(int x[], int y[])
{
	int i;

	for(i=0; i<15; i++)
	{
		objX[i]=x[i];
		objY[i]=y[i];
	}
}*/



int objindex=1;

float cord2Angle(float x1, float x2, float x3, float y1, float y2, float y3) {

	float gradiant1;
	float gradiant2;

	float angle1;

	float angle2;


	if(x2-x1!=0.0)
	{
		gradiant1 = fabs((y2-y1)/(x2-x1));
		angle1 = atan(gradiant1);
	}
	else 
		angle1 = 0.785;
	
	if(x3-x2!=0.0) {
		gradiant2 = fabs((y3-y2)/(x3-x2));
		angle2 = atan(gradiant2);
	}
	else 
		angle2 = 0.785;	
	

	return angle1 + angle2;

}

void sighandler(int signo)
{
	close(cam);
	exit(0);
}


void *checkdist(void) // 적외선 센서 값
{
	int i;
	int flag=0;

	adc=open("/dev/FOUR_ADC", O_RDWR | O_NOCTTY);

	while(1)
	{
		flag=0;

		rxbuf[0]=0, rxbuf[1]=0, rxbuf[2]=0;
		read(adc,rxbuf,sizeof(rxbuf));
		//printf("%d	%d	%d\n",rxbuf[0],rxbuf[1],rxbuf[2]);
		//if(command==26 || command==14 || command==17)
		//	;
		/*else */if(rxbuf[0]>400 || rxbuf[1]>400 || rxbuf[2]>400)
		{	
			for(i=0; i<15; i++)
			{
				rxbuf[0]=0, rxbuf[1]=0, rxbuf[2]=0;
				read(adc,rxbuf,sizeof(rxbuf));
				if(rxbuf[0]<200 && rxbuf[1]<200 && rxbuf[2]<200)
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				findflag=1;
		}
		else 
			findflag=0;

	}
}



void *backBoard(void) //로봇 동작 제어
{	

	cam=open("/dev/ttyS2", O_RDWR | O_NOCTTY);
	if( cam < 0 ) {
		printf("connect backboard error");
		return -1;
	}
	int tmpr=0;
	int tmpCom=17;
	while(1)
	{
		tmpr=0;
		if(compMove==1)
		{
			
		
			write(cam,&command,1);
			read(cam,&tmpr,1);
			compMove = 0;
		}
	}

}


void *findObj(void)
{
	int objx=0;
	//vals
	int len;
	int countx, county;
	int index, cx, cy;
	unsigned char y, cb, cr;
	int redCount=0;
	int stopSign=0;
	int readCom;
	float way;
	int i;
	int prCount=0;

	int nowCnt;
	float nowSeta;

	int colcom;

	//cam and image value
	int handle;
	int handle_overlay2;
	//int nowX, nowY;
	//float ty, tx, rad;
	int nowCount;
	//int startX=0;
	//int startY=0;

	struct pxa_video_buf* vidbuf;
	struct pxacam_setting camset;
	struct pxa_video_buf vidbuf_overlay;

	for(i = 0 ; i < 15 ; i++){
		printf("[%d %d]", objX[i], objY[i]);
	}


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
	
	command=2;
	compMove=0;
	int precommand=3;
	int state=2;
	
	while(1)
	{	
		
		if(state==2)	{
			
			nowSeta = cord2Angle(objX[objindex-1],objX[objindex],objX[objindex+1],objY[objindex-1], objY[objindex], objY[objindex+1]);
			printf("%f\n",nowSeta);
			nowCnt = (3.141592f - nowSeta)/0.2617f;
			

			if(nowSeta - abs(nowSeta) > 0.5f)
				nowCnt++;

			//nowSeta = 180.0 -(nowSeta*180.0f/3.14159f);
			//nowCnt = (int)(nowSeta/15.0);
			printf("%d\n",nowCnt);


			if(objX[objindex] < objX[objindex+1] )
				command = 3;
			else
				command = 1;


		
			count = 0;

			
			while(nowCnt > count) {
				readCom = 0;
				write(cam, &command, 1);	
				read(cam, &readCom, 1);
				if(readCom == command)
					count++;				
		
			}

			objindex++;


			if(objX[objindex] < objX[objindex+1] )
				colcom = 17;
			else
				colcom = 27;
			
			count = 0;
			sleep(1);
			command=2;
			state=0;
			compMove=1;
			continue;

		}
		else {
		vidbuf = camera_get_frame(handle);
		//copy frame to var
		memcpy(vidbuf_overlay.ycbcr.y,vidbuf->ycbcr.y,len);
		memcpy(vidbuf_overlay.ycbcr.cb,vidbuf->ycbcr.cb,len/2);
		memcpy(vidbuf_overlay.ycbcr.cr,vidbuf->ycbcr.cr,len/2);

		countx = 0;
		county = 0;
		int redfind=0;
		int redend=0;
		int redstart=0;
		int tmpcy=1;	
		for( cy = 2 ; cy < 239 ; cy++ ) {
			for( cx = 2 ; cx < 318 ; cx++ ) {
				//cordnate to index
				index = cy * vidbuf_overlay.width + cx;

				//set //ycbcr val
				y = vidbuf_overlay.ycbcr.y[index];
				cb = vidbuf_overlay.ycbcr.cb[index/2];
				cr = vidbuf_overlay.ycbcr.cr[index/2];

				if( 60 < cr - cb && cr > 150 && cb < 130 && 60 < y && 140 > y) {					
					if(redfind==0)
					{
						redfind=1;
						redstart=cx;
						tmpcy=cy;
						break;
					}
				}
			}
			if(redfind==1)
					break;
		}
			
		redfind=0;

		for( cy = 2; cy < 239 ; cy++ ) {
			for( cx = 318 ; cx > 2 ; cx-- ) {
				//cordnate to index
				index = cy * vidbuf_overlay.width + cx;

				//set ycbcr val
				y = vidbuf_overlay.ycbcr.y[index];
				cb = vidbuf_overlay.ycbcr.cb[index/2];
				cr = vidbuf_overlay.ycbcr.cr[index/2];

				if( 60 < cr - cb && cr > 150 && cb < 130 && 60 < y && 140 > y) {
					if(redfind==0)
					{
						redfind=1;
						redend=cx;
						break;
					}
							
				}
			}
			if(redfind==1)
				break;
		}

		redCount=0;

		for(cy=2; cy<239; cy++)	{
			for(cx=redstart; cx<redend; cx++)	{

				index = cy * vidbuf_overlay.width + cx;

				//set ycbcr val
				y = vidbuf_overlay.ycbcr.y[index];
				cb = vidbuf_overlay.ycbcr.cb[index/2];
				cr = vidbuf_overlay.ycbcr.cr[index/2];

				if( 60 < cr - cb && cr > 150 && cb < 130 && 60 < y && 140 > y) {
					redCount++;
					 vidbuf_overlay.ycbcr.y[index]=128;
					 vidbuf_overlay.ycbcr.cb[index/2]=255;
					vidbuf_overlay.ycbcr.cr[index/2]=0;
							
				}
			}
		}
			


		//if(redCount<30)
			//objx=160;
		//if(abs(redend-redstart)>200)
			//objx=160;
		//else
		{
			objx=(redend-redstart)/2;					
			objx+=redstart;
		}
		//pthread_mutex_lock(&mutex);
		if(compMove==1 || count>0)
			;
		else {
			if(command==26)
			{
				command=colcom;
				sleep(1);
			}
			else if(command==colcom)
			{	
				command=14; sleep(1);
			}
			else if(command==14)
			{
				//if(findflag==1)
				//	command=colcom;
				//else
					state=2;

			}
			else if(findflag==1)
				command=26;
			else if(objx<150)	
				command=6;
			else if(objx>170)
				command=4;
			else
				command=5;
			compMove=1;
		}
		//pthread_mutex_unlock(&mutex);

		}

		camera_release_frame(handle,vidbuf);

	}

	//end cam
	camera_stop(handle);
	camera_close(handle);

}





