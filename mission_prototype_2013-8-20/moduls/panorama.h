/*
 *
 * Author:     Chanyoung Kim
 * Created:    2013-8-6
 *
 * get wide view by panorama cam
 * 
 */

#ifndef	_PANORAMA_H_
#define	_PANORAMA_H_

#include <pxa_lib.h>
#include <pxa_camera_zl.h>
#include <math.h>

//struct for combined image
struct combinedImg{
	int width, height;
	enum pxavid_format format;
	union{
		struct{
			//unsigned char *y,*cb,*cr;
			unsigned char y[240*320*2];
			unsigned char cb[320*240];
			unsigned char cr[320*240];
			
		}ycbcr;
	};
};
int makeImg (struct combinedImg* cImg, int imageFlag, int isRightimage, struct pxa_video_buf* buf);
int integrateImg (struct combinedImg* cImg, struct pxa_video_buf* buf);

#endif



