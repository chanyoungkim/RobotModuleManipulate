#include "stereo.h"




int stereo(int* boundS, int* boundC, int* heightS, int* heightC, int* cord) {
//	int sbi, cbi;
	int i , j;
	double wpx, hpx;
	//double wangle, hangle, dist;
	//double w, h;
	//int x, y;
	int cordN;
	float plus_Angle;
	float dist, wDist;
	const float robo_Height = 32;
	const float toRadian = (M_PI/180);
	const float short_H = 20;
	const float long_H = 114;
	const float long_Angle = atan(long_H / robo_Height);	// 라디안
	const float short_Angle = atan(short_H/robo_Height);
	const float a = sqrt(short_H*short_H + robo_Height*robo_Height);
	const float theta = 90*toRadian - (long_Angle-short_Angle)/2;
	const float h_PixerPerCm = (a * cos(theta) * 2) / 240;

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
		wpx = ((double)boundS[i] + (double)boundS[i +1])/2;	
		hpx = heightS[i];
	/*
		//convert bound to angle
		wpx = ((double)boundS[i] + (double)boundS[i +1])/2;
		if( 320 <= wpx && wpx < 480 ) {
			w = (wpx - 320) * 0.24375;
			wangle = 0.485 - atan((39.0-w)/74.0);
		}
		else if( 480 <= wpx ) {
			w = (wpx - 480) * 0.24375;
			wangle = atan(w/74.0) + 0.485;
		}

		
		//convert height to dist
		hpx = heightS[i];
		if( hpx < 120 ) {
			h = hpx * 0.13;
			hangle = 0.37 - atan((15.625-w)/40.26);
		}
		else {
			h = (hpx - 120) * 0.13;
			hangle = atan(h/40.26) + 0.37;
		}
		hangle += 0.736;
		dist = robo_Height/tan(hangle);

		printf("%d[%f %f]\n",i/2, wangle, dist);
		
		//convert (angle, dist) to (x, y)
		//todo confirm cos sin
		x = (int)(dist * cos(wangle));
		y = (int)(dist * sin(wangle));

		//to ary
		cord[i] = x;
		cord[i+1] = y;
		cordN+=2;
	*/
		float k = h_PixerPerCm * hpx;
		float c = sqrt(a*a+k*k-2*a*k*cos(theta));
		float argu = (a*a+c*c - k*k) / (2*a*c);
		plus_Angle = acos(argu);
		float tan_Theta = short_Angle + plus_Angle;
		dist = 32*tan(tan_Theta);
		float half_Width = (dist+27)*(50.0/142);
		float wDist = ((wpx - 160) / 160.0 ) * half_Width;
		wDist *= 1.1;	// 오차보정	

		printf("dist : %f\n", dist);
		printf("wDist : %f\n", wDist);	

	}
	//print cord
	printf("\nc ");
	for(i = 0 ; i < cordN ; i+=2) {
		printf("%2d[%3d, %3d], ", i/2, cord[i], cord[i+1]);
	}printf("\n");
	
	//print map
	
	return 0;
}
