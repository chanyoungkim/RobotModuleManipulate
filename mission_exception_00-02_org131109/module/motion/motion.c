#include "module/motion/motion.h"

#define UNDEFINED_CMD	-1

#define GRAB_MILK	4
#define PUT_TARGET	32

#define SEEDOWN		8
#define SEEFAR    5

#define MOVE_STRAIGHT	11
#define MOVE_RIGHT	13
#define MOVE_LEFT	14
#define STOP		26

#define RIGHTONESTEP	9
#define LEFTONESTEP	7
#define FRONTONESTEP	31

#define TURN180		15
#define TURN180_GRABING	20

#define BACKWARDONESTEP	12

#define MOVE_STRAIGHT_GRABBING	21
#define MOVE_RIGHT_GRABBING	30
#define MOVE_LEFT_GRABBING	28
#define TURN_LEFT_GRABBING 	17
#define TURN_RIGHT_GRABBING	27


void motion(int cmd)
{
  switch(cmd){
    case MOVE_STRAIGHT:
    case MOVE_RIGHT:
    case MOVE_LEFT:
    case MOVE_LEFT_GRABBING:
    case MOVE_RIGHT_GRABBING:
    case MOVE_STRAIGHT_GRABBING:
    case TURN_LEFT_GRABBING:
    case TURN_RIGHT_GRABBING:
    case STOP:
      
    case SEEDOWN:
    case SEEFAR:
         
    case FRONTONESTEP:
    case LEFTONESTEP:
    case RIGHTONESTEP:
    
    case GRAB_MILK:
    case PUT_TARGET:
      
    case TURN180:
    case TURN180_GRABING:
      
    case BACKWARDONESTEP:
	writeCommand(cmd);
	break;
  }  
}

int writeCommand(int cmd)
{
  write(fdBackBoard, &cmd, 1);
  
  return 0;
}

void confirm_Response(){
	while(1)     
	{         
		if(read(fdBackBoard, &response, 1) > 0)	// 읽는데 성공하면 루프 탈출
		{
		//	printf("response message : %d\n", response);
			break;
		}
	} 
}


void moveStop(){
  command = STOP;	// stop
  motion(command);
  command = UNDEFINED_CMD;
  
  confirm_Response();
  
}

void moveLeft(){
  command = MOVE_LEFT;
  motion(command);
  command = UNDEFINED_CMD;
}
void moveRight(){
  command = STOP;	
  motion(MOVE_RIGHT);
  command = UNDEFINED_CMD;
}
void moveStraight(){
  command = MOVE_STRAIGHT;
  motion(command);
  command = UNDEFINED_CMD;
}

void moveStraightGrabbing()
{
  command = MOVE_STRAIGHT_GRABBING;
  motion(command);
  command = UNDEFINED_CMD;
}

void moveLeftGrabbing()
{
  command = MOVE_LEFT_GRABBING;
  motion(command);
  command = UNDEFINED_CMD;
}

void moveRightGrabbing()
{
  command = MOVE_RIGHT_GRABBING;
  motion(command);
  command = UNDEFINED_CMD;
}

void seeDown()
{
  command = SEEDOWN;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response();
}

void seeFar()
{
  command = SEEFAR;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response();
}

void seeUp()
{
  command = SEEDOWN;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response();
}

void rightOneStep()
{
  command = RIGHTONESTEP;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void leftOneStep(){
  command = LEFTONESTEP;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void frontOneStep()
{
  command = FRONTONESTEP;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void grabMilk()
{
  command = GRAB_MILK;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void turn180()
{
  command = TURN180;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void turn180_Grabbing()
{
  command = TURN180_GRABING;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void backwardOneStep()
{
  command = BACKWARDONESTEP;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void putTarget()
{
  command = PUT_TARGET;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void turn_Left_Grabbing()
{
  command = TURN_LEFT_GRABBING;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}

void turn_Right_Grabbing()
{
  command = TURN_RIGHT_GRABBING;
  motion(command);
  command = UNDEFINED_CMD;
  confirm_Response(); 
}
