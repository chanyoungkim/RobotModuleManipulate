#include "stereo.h"

#define ROBOT_HEIGHT 32
#define VERTEX_ANGLE 174.6
#define PROJECT_HEIGHT_MID 15.625
#define WIDTH_CENTI_PER_PIXEL 0.24375
#define HEIGHT_CENTI_PER_PIXEL 0.13
#define CAMERA_TO_PROJECT 40.26
#define ANGLE_OBJECT_CAMERA_ORTHOGONAL 0.37
#define ANGLE_OBJECT_TO_CAMERA 68.2
#define DISTANCE_CAMERA_TO_OBJECT 36.8

Posture stereo(int* boundS, int* boundC, int* heightS, int* heightC, int* cord) {
//	int sbi, cbi;
	int i , j;
	double wpx, hpx;
	double width_angle, height_angle, dist;
	double w, h;
	int x, y;
	int cordN;
	
	Posture pos;

	//init cord
	for(j = 0 ; j < 30 ; j++ )
		cord[j] = 0;
	cordN = 0;

	printf("\nb ");
	for(i = 0 ; boundS[i] != 1000 ; i++ ) {
		printf("%d ", boundS[i]);
	}
	printf("\nh ");
	for(i = 0 ; heightS[i] != 1000 ; i++ ) {
		printf("%d ", heightS[i]);
	}printf("\n");

	//point by S
	for(i = 0 ; boundS[i+1] != 1000 ; i+=2 ) {
		//convert bound to angle
		wpx = ((double)boundS[i] + (double)boundS[i+1])/2;
        w = (wpx - 320)*WIDTH_CENTI_PER_PIXEL;
        //width_angle = VERTEX_ANGLE/360-atan((39.0-w)/74.0);
		width_angle = acos((DISTANCE_CAMERA_TO_OBJECT-w*cos(ANGLE_OBJECT_TO_CAMERA)/sqrt(pow(DISTANCE_CAMERA_TO_OBJECT, 2)+pow(w, 2)-2*DISTANCE_CAMERA_TO_OBJECT*w*cos(ANGLE_OBJECT_TO_CAMERA));
		
		//convert height to dist
		hpx = heightS[i];
		if( hpx < 120 ) {
			h = hpx * HEIGHT_CENTI_PER_PIXEL;
			height_angle = ANGLE_OBJECT_CAMERA_ORTHOGONAL - atan((PROJECT_HEIGHT_MID-h)/CAMERA_TO_PROJECT);
		}
		else {
			h = (hpx - 120) * HEIGHT_CENTI_PER_PIXEL;
			height_angle = atan(h/CAMERA_TO_PROJECT) + ANGLE_OBJECT_CAMERA_ORTHOGONAL;
		}
		hangle += 0.736;
		dist = ROBOT_HEIGHT*atan(height_angle);

		printf("%d[%f %f]\n",i/2, width_angle, dist);
		
		//convert (angle, dist) to (x, y)
		//todo confirm cos sin
		x = (int)(dist * cos(width_angle));
		y = (int)(dist * sin(width_angle));
                            
		pos.x = x;
		pos.y = y;                           
	}
                           
	return pos;
}

