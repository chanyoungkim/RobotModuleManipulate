#ifndef _AI_H_
#define _AI_H_

#include "common.h"
#include "module/motion/motion.h"
#include "module/vision/vision.h"


#define max(a,b) (((a)>(b))? (a):(b))
#define min(a,b) (((a)<(b))? (a):(b))
#define NOISE_COUNT 2




void* doMission(void);


Target search_Milk();

int approach2Milk(VideoCopy *image, Target obj);

Target zone_Search_Handler(VideoCopy *image, Target obj);
void approach2Zone(VideoCopy *image, Target obj);

void alignDirection();
void getAwayFromTargets(void);

void getTarget(Target target); // go and pick
void putObject(void); // go and throw

void go_MiddlePos(int sec);

void zoneHandler(int* minorStep, Target obj);


void generate_Infra();
void terminate_infra();
void *infraredsensor(void);

#endif
