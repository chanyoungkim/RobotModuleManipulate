#include "module/vision/vision.h"

const int tolerance = 70;
const int yMax = 220;

void copyImage()
{
		vidbuf = camera_get_frame(fdCamera);
		
		memcpy(vidbuf_overlay.ycbcr.y,vidbuf->ycbcr.y,len_vidbuf);
		memcpy(vidbuf_overlay.ycbcr.cb,vidbuf->ycbcr.cb,len_vidbuf/2);
		memcpy(vidbuf_overlay.ycbcr.cr,vidbuf->ycbcr.cr,len_vidbuf/2);
		
		memcpy(bufCopy.ycbcr.y,vidbuf->ycbcr.y,len_vidbuf);
		memcpy(bufCopy.ycbcr.cb,vidbuf->ycbcr.cb,len_vidbuf/2);
		memcpy(bufCopy.ycbcr.cr,vidbuf->ycbcr.cr,len_vidbuf/2);
		
		camera_release_frame(fdCamera,vidbuf);
}

Target findTarget(VideoCopy *image) {
	int x,y;
	int countRed,countGreen, countBlue;
	int ball;
	Target obj;
	
	obj.flag=0;
	obj.x_point=0;
	obj.y_point=0;
	
	for(x=1;x<MAX_X;x++){
		countRed=0; countGreen=0; countBlue=0; ball=0;
		for(y=1;y<yMax;y++){

		HSV hsv = ycbcr2HSV(image, x, y);

			if(isColor(hsv, flourscencegreen_B)){	// 빨강이라면
			ball++;
			
		if(ball>NOISE_COUNT){ 
			obj.flag = 10;
			distinguish(&obj, image,x,y,flourscencegreen_B);
			return obj;
		}
			}
			/*
			else if(isColor(hsv, blue_B)){	// 파랑이라면
			countBlue++;
			
		if(countBlue>NOISE_COUNT){ 
			obj.flag = PLASTIC;
			distinguish(&obj, image,x,y,blue_B);
			return obj;
		}
			}
			
			else if(isColor(hsv, green_B)){
			countGreen++;
			
		if(countGreen>NOISE_COUNT){
			obj.flag = MILK;
			distinguish(&obj, image,x, y, green_B);
			return obj;
		}
			}
			*/
			else
	countRed = countGreen = countBlue = ball = 0;
		
		}
	}

	printf("not found\n");
	return obj;
}

void distinguish(Target *obj, VideoCopy* image,int tX,int tY,ColorBoundary color_B)
{
	int minY = tY;
	int minX = tX;
	int x,y;
	int count = 0;
	int num = minX;
	
	for(x=minX;x<MAX_X;x++){
	for(y=0;y<yMax;y++){
		HSV hsv = ycbcr2HSV(image, x, y);
		
		if(isColor(hsv, color_B)){
			count++;
			
			if(count>NOISE_COUNT){
				
				num=x;
				minY = min(minY, y);
				
				break;
			}
		}
		
		else
			count = 0;
	}
	}
	obj->start_x = minX;
	obj->end_x = num;
	obj->x_point = (num+minX)/2;
	obj->y_point = minY;
	printf("obj.x : %d, obj.y : %d, flag : %d\n",obj->x_point,obj->y_point, obj->flag);
}

int toleranceSearch(VideoCopy *image, Target *obj, ColorBoundary color_B) {
	int x, y;
	int start,end;
	int count = 0;
	int past_y = obj->y_point;
	int flag = 0;
	
	//	printf("start : %d, end : %d\n", obj->start_x, obj->end_x);	 
	
	start = obj->start_x-tolerance < 1 ? 1 : obj->start_x-tolerance;
	end = obj->end_x+tolerance > MAX_X ? MAX_X : obj->end_x+tolerance;

	printf("start : %d, end : %d\n", start, end);		
	
	for(x=start;x<end;x++){

		for(y=20;y<yMax;y++){
			HSV hsv = ycbcr2HSV(image, x, y);

			if(isColor(hsv,color_B)){
				count++;

				if(count>NOISE_COUNT){
					if(flag==0){
						obj->start_x = x;
						flag=1;
					} else obj->end_x = x;
					obj->y_point = min(y,obj->y_point);
					break;
				} 
			} else count = 0;
		}
	}
	
	obj->x_point = (obj->start_x + obj->end_x) /2;
/*	
	if(obj->flag == MILK)	// case milk, 위치 보정
	{ 
			HSV hsv = ycbcr2HSV(image, obj->x_point, obj->y_point+20);
			if(!isColor(hsv, green_B))
				flag = 0;
	} 
*/	
	if(flag==0){
		printf("flag == 0, in toleranceSearch \n");
			
		if(past_y < 50)
			return 0;
		
		//	obj->flag = 0;
		return -1; 
	}
	
	return 1;
}

int toleranceZoneSearch(VideoCopy *image, Target *obj, ColorBoundary color_B, int * zoneCnt)
{
	int x, y;
	int start,end;
	int count = 0;
	int past_y = obj->y_point;
	int flag = 0;
	
//	printf("start : %d, end : %d\n", obj->start_x, obj->end_x);	 
	
	start = obj->start_x-tolerance < 1 ? 1 : obj->start_x-tolerance;
	end = obj->end_x+tolerance > MAX_X ? MAX_X : obj->end_x+tolerance;

	printf("start : %d, end : %d\n", start, end);		
//	printf("obj y point : %d\n", obj->y_point);
	for(x=start;x<end;x++){

		for(y=1;y<MAX_Y;y++){
			HSV hsv = ycbcr2HSV(image, x, y);

			if(isColor(hsv,color_B)){
				count++;

				if(count>NOISE_COUNT){
					if(flag==0){
						obj->start_x = x;
						flag=1;
					} else obj->end_x = x;
					obj->y_point = min(y,obj->y_point);
					
					
					
					break;
				} 
			} else count = 0;
		}
	}
	
	obj->x_point = (obj->start_x + obj->end_x) /2;

	// stop Flag
	if(flag ==1)
	{
			x = obj->x_point;
			
			for(y = MAX_Y -1; y>1 ;y--)
			{
		HSV hsv = ycbcr2HSV(image, x, y);
		
		if(isColor(hsv, color_B))
		{
				if(y < 95)	// 경험적으로 이정도 수치면 도착지점 바롤 앞에서 멈춤
				{
					flag = 0;
					printf("y < 95, y = %d\n", y);
				}
			break;
		}
			}
	 
	}
	
	
	if(flag==0){
		printf("flag == 0, in toleranceSearch \n");
			
		if(past_y < 50)
			return 0;
		
	//	obj->flag = 0;
		return -1; 
	}
	
	return 1;
}


Target findZone(VideoCopy *image,Target obj)
{
	int x, y;
	int count;
	
	ColorBoundary color_B = classifyObject(obj.flag);
	Target zone = {0, 0, 0, 0, 0};

	
	for(x=1;x<MAX_X;x++){
		count=0;
		for(y=MAX_Y/3;y<MAX_Y;y++){

		HSV hsv = ycbcr2HSV(image, x, y);

		 		if(isColor(hsv, color_B)){
		 			 count++;
					
			if(count>NOISE_COUNT){ 
				zone.flag = obj.flag;
				distinguish(&zone, image,x,y,color_B);
//				printf("x : %d, y : %d, flag : %d\n", zone.x_point, zone.y_point, zone.flag);
				return zone;
			}
		}
		else
			count=0;
		}
	}
	//TODO fail case
	printf("zone not fount\n");
	return zone;
}

//TODO 리턴된 오브잭트로 toleranceSearch(image,Object,color_B)
//를 메인에서 지속호출하며 놓을 장소로 이


ColorBoundary classifyObject(int flag){
	switch(flag)
	{
		case 1:
			return red_B;
			break;
		case 2:
			return blue_B;
			break;
		case 3:
			return green_B;
			break;
		default:
			return error_B;
			break;
	}
}

