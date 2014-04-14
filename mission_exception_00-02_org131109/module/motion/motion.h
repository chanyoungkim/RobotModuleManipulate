#ifndef _MOTION_H_
#define _MOTION_H_ 

#include "common.h"

/*
int goForward(int dst); // distance FAR/NEAR
int walk(int dir); // direction

// int moveDistance(int dir, );

int turn(int degree);

int grabTarget(Target t);
int throwTarget(); // TODO: Target에 따라?

int stopMovement();
*/


int writeCommand(int cmd);
void motion(int cmd);
void confirm_Response();


void moveStop();
void moveLeft();
void moveRight();
void moveStraight();
void seeDown();
void seeUp();
void seeFar();

void rightOneStep();
void leftOneStep();
void frontOneStep();

void grabMilk();
void putTarget();

void turn180();
void turn180_Grabbing();

void backwardOneStep();


void moveStraightGrabbing();
void moveLeftGrabbing();
void moveRightGrabbing();
void turn_Left_Grabbing();
void turn_Right_Grabbing();

#endif


