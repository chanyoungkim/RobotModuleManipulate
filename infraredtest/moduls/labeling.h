/*
 *
 *
 */

#ifndef	_LABELING_H_
#define	_LABELING_H_

#include <math.h>

#include <pxa_lib.h>
#include <pxa_camera_zl.h>

#include "panorama.h"

#define min3(a,b,c) (((a)>(b)) ? ((((b)>(c)) ? (c):(b))):((((a)>(c)) ? (c):(a))))
#define max3(a,b,c) (((a)>(b)) ? ((((a)>(c)) ? (a):(c))):((((b)>(c)) ? (b):(c))))
#define max2(a,b) (((a)>(b))? (a):(b))
#define min2(a,b) (((a)<(b))? (a):(b))
#define MAX_X	320
#define MAX_Y	240
#define _TRUE_ 1
#define _FALSE_ 0

//function for rabeling

typedef struct{
	float h;
	float s;
	float v;
}HSV;

typedef struct{
    int hmin;
    int hmax;
    int smin;
    int smax;
    int vmin;
    int vmax;
}ColorBoundary;

int verticalProject(struct combinedImg* buf, int* vproj);
int drawProject(struct combinedImg* buf, int* vproj);
int setBoundary(struct combinedImg* buf, int* vproj, int* boundary);
int drawBoundary(struct combinedImg* buf, int* boundary);

int getYVals(struct combinedImg* buf, int* vproj, int* bound, int* heights);
HSV ycbcr2HSV(struct combinedImg* buf,int x, int y);
void ycbcr2rgb(struct combinedImg* buf, int cx, int cy, int* r, int* g, int* b);
int isColor(HSV hsv, ColorBoundary color_B);
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );

int checkBufBoundary(struct combinedImg* buf, int x, int y);
int noiseFiltering(struct combinedImg* buf, int x, int y, ColorBoundary color_B);

//void pre();
#endif

