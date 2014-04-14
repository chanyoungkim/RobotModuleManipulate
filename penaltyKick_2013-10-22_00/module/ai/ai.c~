#include "ai.h"

int positioncount=0;

int timerflag = 1;
int stepCnt = 0;

void *countStepTimer(void) {
	while(timerflag) {
		usleep(8333333);
		stepCnt++;
	}
}

void* doMission(void) {
	KickDiagonalRight();

	usleep(1000000);

	int i;
	for(i = 0; i < 6; i++) {
		rightOneStep();
		usleep(200000);
	}

	for(i = 0; i < 2; i++) {
		
		stepCnt = 0;
		while(1) {
			Target t = findTarget(&bufCopy);
			if(t.flag == 0)
				break;
			//printf("x : %d, y : %d\n", t.x_point, t.y_point);

			int thr_id;
			pthread_t p_threads;
			
			timerflag = 1;
			thr_id = pthread_create(&p_threads, NULL, countStepTimer, (void *)NULL);     
			if (thr_id < 0) {    
				perror("countStepTimer create error : ");  
				exit(0);    
			}
			
			approach2Target(&bufCopy, t);
			timerflag = 0;

			int status;
			pthread_join(p_threads, (void **)&status);

			printf("%d step\n", stepCnt);
		}
		
		// #ifdef DEBUG_NOACTION

		int kickFlag = 0;
		fineControl(&bufCopy, &kickFlag);
		
		if(i==0) {
			if (stepCnt < 5) {
				if(kickFlag == 1)
					moderatekickRight();
				else if(kickFlag == -1)
					moderatekickLeft();
			} else {
				if(kickFlag == 1)
					kickRight();
				else if(kickFlag == -1)
					kickLeft();
			}
		} else if(i==1) {
			if(kickFlag == 1)
				strongkickRight();
			else if(kickFlag == -1)
				strongkickLeft();
		}

		// #endif

	}
	
	//	KickToGoalArea();
	/*
		while(1){
			int i, j;
			
			for(i = 155; i<165; i++)
			{
		for(j=230; j<239; j++)
		{
				HSV hsv = ycbcr2HSV(&bufCopy, i, j);
				
				printf("%f, %f, %f\n", hsv.h, hsv.s, hsv.v);
				
		}
			}
			printf("--------------------------------------\n");
		}
	*/	
}


int approach2Target(VideoCopy *image, Target obj) {
	
	ColorBoundary color_B = flourscencegreen_B;
	
	
	int isFind;
	while(1)
	{
		// search
		isFind = toleranceSearch(image, &obj, color_B);
		printf("isFind : %d\n", isFind);
		
		if(isFind <= 0)	// stop_Signal
			break;
		
		// command -> robot
		else{
			printf("x_mid : %d\n", obj.x_point);
			
			int wPix = (MAX_X/2 - obj.x_point);
			printf("wPix : %d, y : %d\n", wPix, obj.y_point);
		
			int direction = wPix < 0 ? 0 : 1;	
			wPix = abs(wPix);
		
			if(wPix > 60){ 
				if(direction == 0)
					moveLeft();
				else if(direction == 1)
					moveRight();
			}
			
			else{	
				moveStraight();
			}
		}
	}
	
	moveStop();
	printf("approach complete. isFind : %d\n", isFind);
}

void alignBetweenObstacle(VideoCopy* image) {
	int found,areaPoint;
	int i;	
	found = 0;

	/*
	while(1){
		if(InAreaPoint(image, blue_B)){ 
				rightOneStep();
			positioncount++;
			//writeCommand(20);	
			//responseCommand();
		}
		else{
			//moveStop();
			//writeCommand(26);		
			//responseCommand();
			break;
		}
	}
	*/
	for(i=0; i<5; i++){
		rightOneStep();
		usleep(200000);
		positioncount++;
	}
}

int fineControl(VideoCopy* image, int* kickFlag){
	while(1) {
		seeDown();
		usleep(1000000);

		Target obj = findTarget(image);		
		printf("x : %d, y : %d\n", obj.x_point, obj.y_point);
		
		seeUp();
		usleep(1000000);

		// x 는 95 ~ 115 로 맞추는것이 오른발 킥 할 때 최적이라고 측정
		// positioncount 는 일단 고려하지 않음
		int flag = 0;

		if(obj.x_point < 160)
		{		
			if(obj.x_point < 90)
			{
				slightlyRightOneStep();
				usleep(500000);	
				flag = 1;		
			}		
			else if(obj.x_point > 120)
			{		
				slightlyLeftOneStep();	
				usleep(500000);	
				flag = 1;	
			}
			*kickFlag = 1;
		}
		else
		{
			if(obj.x_point < 200)
			{
				slightlyRightOneStep();
				usleep(500000);	
				flag = 1;		
			}		
			else if(obj.x_point > 230)
			{		
				slightlyLeftOneStep();	
				usleep(500000);	
				flag = 1;	
			}
			*kickFlag = -1;
		}
		
		if(obj.y_point > 5)
		{
			slightlyFrontOneStep();		
			usleep(500000);
			flag = 1;	
		}

		if(flag == 0)
			break;
	}
	
//	printf("end!!\n");
}

/*
void KickToGoalArea(){
	if(positioncount>0)
		leftDiagonalKick();
	else if(positioncount<0)
		rightDiagonalKick();
	else
		rightKick();
}
*/

int InAreaPoint(VideoCopy* buf,ColorBoundary color_B){
	int temp;
	int ball = 0;
	int x, y;
	int i;
	for(x=120; x<200; x++){
		for(y=319; y>1; y--)
		{
				HSV hsv = ycbcr2HSV(buf, x, y);
				
				
				if(isColor(hsv, color_B))
				{
			ball++;
			
			if(ball > NOISE_COUNT)
					return 1;
			
				}
				else
					ball = 0;
		
				
		}
	}
	/*
	for(x=start; x<end; x++){
		y = END_Y;		
		HSV hsv = ycbcr2HSV(buf, x, y);

		 if(!(*found) && isColor(hsv, color_B) && noiseFiltering(buf, x, y, color_B)){		// 색깔을 만나면
		return 1; 
		} 
	}

	for(y = START_Y; y>END_Y; y--){
		x = start;
		HSV hsv = ycbcr2HSV(buf, x, y);

	 if(!(*found) && isColor(hsv, color_B) && noiseFiltering(buf, x, y, color_B)){		// 색깔을 만나면
		return 1; 
		} 
	}

	for(y = START_Y; y>END_Y; y--){
		x = end;
		HSV hsv = ycbcr2HSV(buf, x, y);

		if(!(*found) && isColor(hsv, color_B) && noiseFiltering(buf, x, y, color_B)){		// 색깔을 만나면
		return 1; 
		}
	}
	*/
	return 0;
}

