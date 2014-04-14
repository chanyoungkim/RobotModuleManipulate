#include "ai.h"

int thr_id, status;
pthread_t p_threads;
int infraStopFlag;
int infraFlag;

int majorStep = 1; // 0_get 1_put
int minorStep = 1;
int step = 1;
int firstStep = 1;

int goal = MILK;

Target prevFinishedZone;

void* doMission(void) {
	prevFinishedZone.flag = MILK; // -1

	int milk_Num = 2;
	if(goal == MILK)
	{
	    while (stopFlag == 0) {
		  switch (step) {
		  case 1:	// 우유팩만 탐색
			  // 우유팩만 찾으면 됨
			  //t = findTarget(&bufCopy);
			  t = findTarget(&bufCopy, COKE);
			  
			  if(milk_Num == 2)
			  {
			      // 못찾을경우 추가적인 움직임과 탐색 필요
			      if(t.flag != 3)
			      {
				  step--;
				  turn_Right_Grabbing();
				  usleep(200000);
			      }
			  }                                               
			  
			  break;
		  case 2:	// 우유팩에 접근	  
			  approach2Milk(&bufCopy, t);
			
			  break;
		  case 3:	// 우유팩 줍기
			  
			  // 우유팩을 위한 겟타겟
//			  t.flag = MILK;
			  getTarget(t);
			  
			  // 180도 돌 것인가?
			  if(milk_Num == 2)
			  {
			      turn180_Grabbing();
			      usleep(200000);
			  }
			    
			  break;
		  case 4:	// 중간까지 나오기
			  if(milk_Num == 1)
			    go_MiddlePos(20);
			  else if(milk_Num == 2)
			  {}
			  
			  break;	
		  case 5:	// 목표장소 탐색
			  // 고동완이 만들 목적지 탐색 비교 함수 필요
			  // 목적지를 찾을 수 있게 좀 나와야함
			  // findZone implementation
//			  t.flag = MILK;
			  z = zone_Search_Handler(&bufCopy, t);
//				zoneHandler(&minorStep, z);
			  if(z.flag == 4 || z.flag == 5 || z.flag == 6) {
			  	step--;
			  } else {
			  	prevFinishedZone = z; // 백업
			  }
			  
			  
			  break;
		  case 6:	// 우유팩 장소에 접근
			  // 우유팩 존에 다가갈 함수 필요
		    
			  approach2Zone(&bufCopy, z);
			  
			  break;
			  
		  case 7:	// 던지기
			  // 우유팩 던지는것 필요
			  putTarget();
			  usleep(200000);
			  milk_Num++;
//				stopFlag = 1;	
			  // 물건 놓았으면 적외선 감지 끝
//				terminate_infra();
			  break;
			  
		  case 8:
			  // 방향보정 + 180도 돌기
			  turn180();
			  usleep(200000);
			  
			  break;
		  default:
			  break;
		  }
		    
		  step++;
		  
		  if(step >= 10)
		    step = 1;
	    }	
	}
/*	
	else if(goal == PLASTIC)
	{
	  while (stopFlag == 0) {
	      
		    switch (minorStep) {
			    case 1:
				    // 플라스틱 찾을 함수 필요
				    //t = findTarget(&bufCopy);
				    t = search_Step();
			      
				    break;
			    case 2:	// 타겟 찾기 예외처리	
				    
				    break;
			    case 3:
				    // 플라스틱에 특화된 접근 필요
				    stopCnt = 0;
				    
				    while(stopCnt < 2){
				      approach2Target(&bufCopy, t);
				      stopCnt++;
				    }
				    
				    break;
			    case 4:	// 물체 접근 예외처리	
				    
				    break;	
			    case 5:
				    // 플라스틱 목적지 탐색하면
				    // 플라스틱 걷어차는 함수 필요
				    getTarget(t);
				    
				    // 성공하였을 경우 적외선 감지 시작
    //				generate_Infra();
				    
				    break;
			    case 6:	// 물체 집기 예외처리
			      
				    break;
				    
			    case 7:	// 다음 단계 전환 전 준비
			      
				    break;
			    default:
				    break;
		    }
		    
		    minorStep++;
	    }
	}
*/	
	// 다 끝나면 캔에 대해서 작업
}

Target search_Milk()
{
    const int maxB = 200;
    const int minB = 120;
  
    Target t;
    
    t = findTarget(&bufCopy, MILK);
    
    if(t.flag > 0)
    {
	while(1)
	{
	    printf("x_point = %d\n", t.x_point); 
	  
	    int refind_Pos_X = t.start_x;
	    
	    if(t.flag == 0)
	      break;
	    
	    if(t.x_point < maxB && t.x_point > minB)
	    {
	      break;
	    }
	    
	    else if(t.x_point >= maxB)
	    {
		leftOneStep();
		refind_Pos_X -= 30;
		if(refind_Pos_X < 1)
		  refind_Pos_X = 1;
	      
	    }
	    
	    else if(t.x_point <= minB)
	    {
		rightOneStep();
		refind_Pos_X += 15;
		if(refind_Pos_X >318)
		  refind_Pos_X = 319;
	    }
	    
	    // usleep(100000);
	    
	    //t = refind(&bufCopy, t.flag, refind_Pos_X);
	    t = findTarget(&bufCopy, MILK);
	}
	
    }
    
    return t;
}

int approach2Milk(VideoCopy *image, Target obj) {
  
  ColorBoundary color_B = classifyObject(obj.flag);
  
  
  int isFind;
  int cnt = 0;

  
  while(1)
  {
	// search
	isFind = toleranceSearch(image, &obj, color_B);
	
	//printf("isFind : %d\n", isFind);
	printf("y : %d\n", obj.y_point);
	
	if(isFind <= 0)	// stop_Signal
	{
	    cnt++;
	    if(cnt > 0)
	      break;
	}
	
//	if(obj.y_point < 20)
//	  break;
	
	// command -> robot
	else{
//	  printf("pass command\nx_mid : %d\n", obj.x_point);
	  
	  int wPix = (MAX_X/2 - obj.x_point);
//	  printf("wPix : %d\n", wPix);
	
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

Target zone_Search_Handler(VideoCopy *image, Target obj)
{
    Target zone = findZone(image, obj);
  
    printf("obj flag : %d\n", zone.flag);
    
    if(zone.flag == 4)	// left turn
    {
	turn_Left_Grabbing();
    }
    else if(zone.flag == 5)	// right turn
    {
	turn_Right_Grabbing();
    }
    
    else if(zone.flag == 6)
    {
		printf("zone not found\n"); 
		if (prevFinishedZone.flag == COKE)
		{
			if (obj.flag == COKE) {
				seeFar();
			} else {
				turn_Right_Grabbing();
			}
		} else if (prevFinishedZone.flag == MILK) {
			if (obj.flag == MILK) {
				seeFar();
			} else {
				turn_Left_Grabbing();
			}
		} else {
			//blue
			// use move LR cnt?
		}
    }
    
    usleep(200000);
    
    return zone;
}

void approach2Zone(VideoCopy *image, Target obj)
{
    ColorBoundary color_B = classifyObject(obj.flag);
   
    int zoneCnt = 0;
    int isFind;
    while(1)
    {
	//search
      isFind = toleranceZoneSearch(image, &obj, color_B, &zoneCnt);
      printf("zone find : %d\n" , isFind);
      
      if(isFind <=0)
	break;
      
      else
      {
	 int wPix = (MAX_X/2 - obj.x_point);
	  printf("wPix : %d\n", wPix);
	
	  int direction = wPix < 0 ? 0 : 1;	
	  wPix = abs(wPix);
	
	  if(wPix > 60){ 
	    if(direction == 0)
	      moveLeftGrabbing();
	    else if(direction == 1)
	      moveRightGrabbing();
	  }
	  else{	
	    moveStraightGrabbing();
	  }
      }
    }
    
    usleep(5000000);	// 경험적 의무걸음
    
    moveStop();
}


void getAwayFromTargets(void) {
    
}


// go and pick
void getTarget(Target target) {
  ColorBoundary color_B = classifyObject(target.flag);
  
  printf("\n--------------------see down------------------\n\n");
  
  
  while(1){
		printf("see down\n");
		// 내려다 보기
		seeDown();
		
		usleep(200000);	// delay 1sec, camera buffer update
		 

		// 내려다 본 결과로 판단
		int frontCnt=0;
		int LR_Cnt=0;
		
	
		Target obj = findTarget(&bufCopy, target.flag);
		
		printf("x : %d, y : %d, flag : %d\n", obj.x_point, obj.y_point, obj.flag);
		
		
		if(obj.flag == COKE){// case coke
		    if(obj.y_point > 80)	
			    frontCnt++;
			    
		    int x = obj.x_point - MAX_X/2;
		    
		    if (x > 50) //	left
		      LR_Cnt--;
		    else if(x < -50) // right
		      LR_Cnt++;
		}
		else if(obj.flag == PLASTIC)
		{}
		else if(obj.flag == MILK)
		{
		  if(obj.y_point > 70)	
			    frontCnt++;
			    
		    int x = obj.x_point - MAX_X/2;
		    
		    if (x > 80) 
		      LR_Cnt++;
		    else if(x < -80) 
		      LR_Cnt--;
		  
		}
		else
		{}
		
		printf("%d %d\n", frontCnt, LR_Cnt);
		printf("see up\n");
		// 몸을 다시 일으키고
		seeUp();
		usleep(200000);
		
		if(frontCnt ==0 && LR_Cnt ==0)	// 보정할거 없으면 탈출
			break;
		
		// 위치 보정
		if(LR_Cnt > 0){
			rightOneStep();
		}
		else if(LR_Cnt < 0){
			leftOneStep();
		}

		if(frontCnt >0){
			frontOneStep();
		}
			
		// 다시 내려다보기, 루프 반복
	}
	printf("\n-------------!grab-----------------\n\n");
	
	//if()
//	grabMilk();
	
	usleep(200000);
}

void go_MiddlePos(int sec)
{
    moveStraightGrabbing();
    
    sec = sec* 1000000;
    usleep(sec);
    
    moveStop();
}

void zoneHandler(int* minorStep, Target obj)
{
    if (obj.flag == 0)
    {
	(*minorStep)--;
	
	moveStraightGrabbing();
	
	usleep(3000000);	//delay 3sec
	
	moveStop();
    }
}

// go and throw
void putObject(void) {

      putTarget();
}

void generate_Infra()
{
      infraStopFlag = 0;
      
      thr_id = pthread_create(&p_threads, NULL, infraredsensor, (void *)NULL);
      if(thr_id < 0)
      {
	perror("infraredsensor create error");
	exit(0);
      }
      
}

void terminate_infra()
{
  infraStopFlag = 1;
  
  pthread_join(p_threads, (void**)&status);
}


void *infraredsensor(void){

    int i, flag;

    static unsigned short rxbuf[2];    


    while(1){

        flag=0;
        
        rxbuf[0]=0;
        rxbuf[1]=0;
        
        
        read(fdInfra, rxbuf, sizeof(rxbuf));
        
	printf("rxbuf[0], rxbuf[1] : %d, %d\n", rxbuf[0], rxbuf[1]);
        if(rxbuf[0]>400 || rxbuf[1]>400){
            for(i=0; i<15; i++){
                rxbuf[0]=0;
                rxbuf[1]=0;
                
                read(fdInfra, rxbuf, sizeof(rxbuf));
		
                if(rxbuf[0]<180 && rxbuf[1]<180){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
                infraFlag=1;
        }
        else
            infraFlag = 0;
	
	//printf("infra : %d\n", infraFlag);
	
	if(infraStopFlag == 1)
	  break;
    }
    
    exit(0);
}


