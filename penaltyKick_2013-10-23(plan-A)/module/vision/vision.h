#ifndef _VISION_H_
#define _VISION_H_

#include <string.h>
#include "common.h"

#define max(a,b) (((a)>(b))? (a):(b))
#define min(a,b) (((a)<(b))? (a):(b))

#define MAX_3(a, b, c)	( ((a)>(b)) ? ( (((a)>(c)) ? (a) : (c)) ) : ( (((b)>(c)) ? (b) : (c)) ) )
#define MIN_3(a, b, c)	( ((a)>(b)) ? ( (((b)>(c)) ? (c) : (b)) ) : ( (((a)>(c)) ? (c) : (a)) ) )

#define NOISE_COUNT 2

#define COKE 	1
#define PLASTIC	2
#define MILK	3

typedef struct {
  int hmin;
  int hmax;
  int smin;
  int smax;
  int vmin;
  int vmax;
} ColorBoundary;

typedef struct {
  float h;
  float s;
  float v;
} HSV;

extern const ColorBoundary blue_B;
extern const ColorBoundary red_B;
extern const ColorBoundary yellow_B;
extern const ColorBoundary green_B;
extern const ColorBoundary error_B;
extern const ColorBoundary flourscencegreen_B;

int isColor(HSV hsv, ColorBoundary color_B);
HSV ycbcr2HSV(VideoCopy* buf,int x, int y);
void ycbcr2rgb(VideoCopy* buf, int cx, int cy, int* r, int* g, int* b);
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );

/*
void doSomethingAboutRedColor(VideoCopy* image, int n);
void doSomethingAboutGreenColor(VideoCopy* image, int n);
*/

void calcul_Dist(); // 거리계산 함수

void copyImage();

Target findTarget(VideoCopy *image);
void distinguish(Target *obj, VideoCopy* image,int tX,int tY,ColorBoundary color_B);

ColorBoundary classifyObject(int flag);

int toleranceSearch(VideoCopy *image, Target *obj, ColorBoundary color_B);
int toleranceZoneSearch(VideoCopy *image, Target *obj, ColorBoundary color_B, int * zoneCnt);

Target findZone(VideoCopy* image, Target obj);

#endif


