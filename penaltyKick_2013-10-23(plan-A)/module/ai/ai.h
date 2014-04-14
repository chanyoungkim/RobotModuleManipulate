#ifndef _AI_H_
#define _AI_H_

#include "common.h"
#include "module/motion/motion.h"
#include "module/vision/vision.h"

#define max(a,b) (((a)>(b))? (a):(b))
#define min(a,b) (((a)<(b))? (a):(b))
#define NOISE_COUNT 2


void* doMission(void);

int approach2Target(VideoCopy *image, Target obj);

void alignBetweenObstacle(VideoCopy* image);

int InAreaPoint(VideoCopy* buf,ColorBoundary color_B);

int fineControl(VideoCopy* image, int* kickFlag);

void getToShootingZone(VideoCopy* image, Target obj);

//void KickToGoalArea();
#endif


