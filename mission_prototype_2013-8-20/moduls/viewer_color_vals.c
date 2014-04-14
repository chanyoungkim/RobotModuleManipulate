/*
 *
 * Author:     Jeang Hasang
 * Created:    2012-7-17
 *
 * Sample code interface header for PXA Linux release. All interfaces
 * introduced in this file is not multiple thread safe unless it's 
 * specified explicitly. 
 *
 */



#include <stdio.h>
#include <signal.h>

#include <pxa_lib.h>
#include <pxa_camera_zl.h>

//#include "functions/ip_get_direction.h"

// VIDEOBUF_COUNT must be larger than STILLBUF_COUNT
#define VIDEOBUF_COUNT  3
#define STILLBUF_COUNT  2

#define CAM_STATUS_INIT     0
#define CAM_STATUS_READY    1
#define CAM_STATUS_BUSY     2

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

int fd;
struct sigaction act;
void sighandler(int signo)
{
	close(fd);
	exit(0);
}

//int ipGetDirection(pxa_video_buf* src);

int main(int argc, char* argv[])
{
	//cam and image value
	int handle;
	int handle_overlay2;
	struct pxa_video_buf* vidbuf;
	struct pxacam_setting camset;
	struct pxa_video_buf vidbuf_overlay;

	//vals
	int len;
	int index, cx, cy;
	int thresholdR, thresholdG, thresholdB;
	unsigned char* r, *g, *b;
	unsigned char y, cb, cr;
	int command;
	int running;

	//connect whit backboard
	fd=open("/dev/ttyS2", O_RDWR | O_NOCTTY);
	if( fd < 0 ) {
		printf("connect backboard error");
		return -1;
	}

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
	
	//alloc memory for rgb
	r = (unsigned char*)malloc(sizeof(unsigned char)*len);
	g = (unsigned char*)malloc(sizeof(unsigned char)*len);
	b = (unsigned char*)malloc(sizeof(unsigned char)*len);

	//infi loop
	while(1){
		//get frame from cam
		vidbuf = camera_get_frame(handle);
	
		//copy frame to var
    		memcpy(vidbuf_overlay.ycbcr.y,vidbuf->ycbcr.y,len);
		memcpy(vidbuf_overlay.ycbcr.cb,vidbuf->ycbcr.cb,len/2);
		memcpy(vidbuf_overlay.ycbcr.cr,vidbuf->ycbcr.cr,len/2);

		/*
		 * do someting by image
		*/
		for( cy = 0 ; cy < 240 ; cy++ ) {
			for( cx = 0 ; cx < 320 ; cx++ ) {
				//cordnate to index
				index = cy * vidbuf_overlay.width + cx;

				//set ycbcr val
				y = vidbuf_overlay.ycbcr.y[index];
				cb = vidbuf_overlay.ycbcr.cb[index/2];
				cr = vidbuf_overlay.ycbcr.cr[index/2];

				r[index] = y;
				g[index] = cb;
				b[index] = cr;

				//todo : handle white reflex
				//yellow
				if( 60 < cr - cb && cb < 80  && cr < 160 && 100 < y ) {
					vidbuf_overlay.ycbcr.y[index] = 140;
					vidbuf_overlay.ycbcr.cb[index/2] = 240;
					vidbuf_overlay.ycbcr.cr[index/2] = 16;
				}
				//red
				else if( 60 < cr - cb && cr > 180 && cb < 140 && y < 100 ) {
					vidbuf_overlay.ycbcr.y[index] = 140;
					vidbuf_overlay.ycbcr.cb[index/2] = 16;
					vidbuf_overlay.ycbcr.cr[index/2] = 240;
				}
				//else
				else {
					vidbuf_overlay.ycbcr.y[index] = 16;
					vidbuf_overlay.ycbcr.cb[index/2] = 128;
					vidbuf_overlay.ycbcr.cr[index/2] = 128;
				}
			}
		}
		/*
		 * end do
		*/

		//print to lcd
		camera_release_frame(handle,vidbuf);

		//print midle pixel info
		printf("%d %d %d\n", (int)r[(vidbuf_overlay.width * vidbuf_overlay.height / 2) +
						 (vidbuf_overlay.width / 2)], 
					(int)g[(vidbuf_overlay.width * vidbuf_overlay.height / 2) +
						 (vidbuf_overlay.width / 2)], 
					(int)b[(vidbuf_overlay.width * vidbuf_overlay.height / 2) +
						 (vidbuf_overlay.width / 2)]);
	}//end while

	camera_stop(handle);
	camera_close(handle);

	return 0;
}
