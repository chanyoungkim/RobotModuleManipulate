#include "angle_recognition.h"

float angleRecognition (struct pxa_video_buf* vidbuf_overlay)
{
	int countx, county;
	int index, cx, cy;
	unsigned char y, cb, cr;
	angleVector direction;
	int last_tmpy;
	int tmpy;
	float way;



	//init angleVector
	direction.start.x = 1000;
	direction.start.y = 1000;
	direction.end.x = 1000;
	direction.end.y = 1000;
	//in 3/1 of img (320, 80) -> (\_/ h40 b40 t85 )
	//have to remove edge vals  and think about cross
	
	//wait for done
	//read(mrc3024, &command, sizeof(command));



//먼저 1/3 부분을 찾는다!!
//------------------------------------------------------------------------------------------------------
	countx = 0;
	county = 0;
	tmpy = 0;
	last_tmpy = 0;
	for( cx = 2 ; cx < 319 ; cx++ ) {
		for( cy = 2 ; cy < 80 ; cy++ ) {
			//cordnate to index
			index = cy * vidbuf_overlay->width + cx;

			//set ycbcr val
			y = vidbuf_overlay->ycbcr.y[index];
			cb = vidbuf_overlay->ycbcr.cb[index/2];
			cr = vidbuf_overlay->ycbcr.cr[index/2];
			//if yellow
			if( (60 < cr - cb && cb < 80  && cr < 160 && 60 < y) ) {
				//vidbuf_overlay->ycbcr.y[index] = 0;
				//vidbuf_overlay->ycbcr.cb[index/2] = 125;
				//vidbuf_overlay->ycbcr.cr[index/2] = 125;
				tmpy += cy;
				last_tmpy = cy;
				if( county < 41)
					county++;
			}
		}

		if( county > 40 ) {
			direction.start.x = cx;
			direction.start.y = (tmpy/county > last_tmpy) ? last_tmpy : tmpy/county;

			//printf("[start: %d %d]", direction.start.x, direction.start.y);
			break;
		}
	}
//------------------------------------------------------------------------------------------------------	
	countx = 0;
	county = 0;
	tmpy = 0;
	last_tmpy = 0;
	for( cx = 318 ; cx > 1 ; cx-- ) {
		for( cy = 80 ; cy >= 2 ; cy-- ) {
			//cordnate to index
			index = cy * vidbuf_overlay->width + cx;

			//set ycbcr val
			y = vidbuf_overlay->ycbcr.y[index];
			cb = vidbuf_overlay->ycbcr.cb[index/2];
			cr = vidbuf_overlay->ycbcr.cr[index/2];
			//if yellow
			if( (60 < cr - cb && cb < 80  && cr < 160 && 60 < y ) ) {
				//vidbuf_overlay->ycbcr.y[index] = 0;
				//vidbuf_overlay->ycbcr.cb[index/2] = 125;
				//vidbuf_overlay->ycbcr.cr[index/2] = 125;
				tmpy += cy;
				last_tmpy = cy;

				if( county < 41)
					county++;
			}
		}

		if( county > 40 ) {
			direction.end.x = cx;
			direction.end.y = (tmpy/county > last_tmpy) ? last_tmpy : tmpy/county;
			//printf("[end: %d, %d]\n", direction.end.x, direction.end.y);
			break;
		}
	}	
//------------------------------------------------------------------------------------------------------






//못찾았으면 2/3 부분을 찾는다!!
//------------------------------------------------------------------------------------------------------
	countx = 0;
	county = 0;
	tmpy = 0;
	last_tmpy = 0;
	if( direction.start.x == 1000 && direction.start.y == 1000 ) 
	{
		countx = 0;
		county = 0;
		for( cx = 2 ; cx < 319 ; cx++ ) {
			for( cy = 80 ; cy < 160 ; cy++ ) {
				//cordnate to index
				index = cy * vidbuf_overlay->width + cx;

				//set ycbcr val
				y = vidbuf_overlay->ycbcr.y[index];
				cb = vidbuf_overlay->ycbcr.cb[index/2];
				cr = vidbuf_overlay->ycbcr.cr[index/2];
				//if yellow
				if( (60 < cr - cb && cb < 80  && cr < 160 && 60 < y)
					|| ( 200 < y && y < 220 && 110 < cb && cb < 130 && 110 < cr && cr < 130 ) ) {
					//vidbuf_overlay->ycbcr.y[index] = 0;
					//vidbuf_overlay->ycbcr.cb[index/2] = 125;
					//vidbuf_overlay->ycbcr.cr[index/2] = 125;
					tmpy += cy;
					last_tmpy = cy;
					if( county < 41)
						county++;
				}
			}

			if( county > 40 ) {
				direction.start.x = cx;
				direction.start.y = (tmpy/county > last_tmpy) ? last_tmpy : tmpy/county;
				//printf("[start: %d %d]", direction.start.x, direction.start.y);
				break;
			}
		}
	
//------------------------------------------------------------------------------------------------------
		countx = 0;
		county = 0;
		tmpy = 0;	
		last_tmpy = 0;
		for( cx = 318 ; cx > 1 ; cx-- ) {
			for( cy = 159 ; cy >= 80 ; cy-- ) {
				//cordnate to index
				index = cy * vidbuf_overlay->width + cx;

				//set ycbcr val
				y = vidbuf_overlay->ycbcr.y[index];
				cb = vidbuf_overlay->ycbcr.cb[index/2];
				cr = vidbuf_overlay->ycbcr.cr[index/2];
				//if yellow
				if( (60 < cr - cb && cb < 80  && cr < 160 && 60 < y ) 
					|| ( 200 < y && y < 220 && 110 < cb && cb < 130 && 110 < cr && cr < 130 ) ) {
					//vidbuf_overlay->ycbcr.y[index] = 0;
					//vidbuf_overlay->ycbcr.cb[index/2] = 125;
					//vidbuf_overlay->ycbcr.cr[index/2] = 125;
					tmpy += cy;
					last_tmpy = cy;
					if( county < 41)
						county++;
				}
			}

			if( county > 40 ) {
				direction.end.x = cx;
				direction.end.y = (tmpy/county > last_tmpy) ? last_tmpy : tmpy/county;
				//printf("[end: %d, %d]\n", direction.end.x, direction.end.y);
				break;
			}
		}
	}
//------------------------------------------------------------------------------------------------------








//나머지 부분을 찾는다!!
//------------------------------------------------------------------------------------------------------
	countx = 0;
	county = 0;
	tmpy = 0;
	last_tmpy = 0;
	if( direction.start.x == 1000 && direction.start.y == 1000 ) 
	{
		for( cx = 2 ; cx < 319 ; cx++ ) {
			for( cy = 160 ; cy < 238 ; cy++ ) {
				//cordnate to index
				index = cy * vidbuf_overlay->width + cx;

				//set ycbcr val
				y = vidbuf_overlay->ycbcr.y[index];
				cb = vidbuf_overlay->ycbcr.cb[index/2];
				cr = vidbuf_overlay->ycbcr.cr[index/2];
				//if yellow
				if( (60 < cr - cb && cb < 80  && cr < 160 && 60 < y)
					|| ( 200 < y && y < 220 && 110 < cb && cb < 130 && 110 < cr && cr < 130 ) ) {
					//vidbuf_overlay->ycbcr.y[index] = 0;
					//vidbuf_overlay->ycbcr.cb[index/2] = 125;
					//vidbuf_overlay->ycbcr.cr[index/2] = 125;
					tmpy += cy;
					last_tmpy = cy;
					if( county < 41)
						county++;
				}
			}

			if( county > 40 ) {
				direction.start.x = cx;
				direction.start.y = (tmpy/county > last_tmpy) ? last_tmpy : tmpy/county;
				//printf("[start: %d %d]", direction.start.x, direction.start.y);
				break;
			}
		}
//------------------------------------------------------------------------------------------------------	
		countx = 0;
		county = 0;
		tmpy = 0;
		last_tmpy = 0;
		for( cx = 318 ; cx > 1 ; cx-- ) {
			for( cy = 237 ; cy >= 160 ; cy-- ) {
				//cordnate to index
				index = cy * vidbuf_overlay->width + cx;

				//set ycbcr val
				y = vidbuf_overlay->ycbcr.y[index];
				cb = vidbuf_overlay->ycbcr.cb[index/2];
				cr = vidbuf_overlay->ycbcr.cr[index/2];
				//if yellow
				if( (60 < cr - cb && cb < 80  && cr < 160 && 60 < y ) 
					|| ( 200 < y && y < 220 && 110 < cb && cb < 130 && 110 < cr && cr < 130 ) ) {
					//vidbuf_overlay->ycbcr.y[index] = 0;
					//vidbuf_overlay->ycbcr.cb[index/2] = 125;
					//vidbuf_overlay->ycbcr.cr[index/2] = 125;
					tmpy += cy;
					last_tmpy = cy;
					if( county < 41)
						county++;
				}
			}

			if( county > 40 ) {
				direction.end.x = cx;
				direction.end.y = (tmpy/county > last_tmpy) ? last_tmpy : tmpy/county;
				//printf("[end: %d, %d]\n", direction.end.x, direction.end.y);
				break;
			}
		}
	}
//------------------------------------------------------------------------------------------------------		

	//아직 시작점 끝점을 못찾았다면 ㅠ
	if( direction.start.x == 1000 && direction.start.y == 1000 ) 
	{
		way = 9999;
		printf("not found T.T");
	}
	//찾았다면!
	else
	{
		//시작점이랑 끝점 표시하기! 				
		index = direction.start.y * vidbuf_overlay->width + direction.start.x;
	
		vidbuf_overlay->ycbcr.y[index+1] = 0;
		vidbuf_overlay->ycbcr.cb[(index+1)/2] = 0;
		vidbuf_overlay->ycbcr.cr[(index+1)/2] = 255;
		vidbuf_overlay->ycbcr.y[index] = 0;
		vidbuf_overlay->ycbcr.cb[index/2] = 0;
		vidbuf_overlay->ycbcr.cr[index/2] = 255;
		vidbuf_overlay->ycbcr.y[index-1] = 0;
		vidbuf_overlay->ycbcr.cb[(index-1)/2] = 0;
		vidbuf_overlay->ycbcr.cr[(index-1)/2] = 255;				
			
		index = direction.end.y * vidbuf_overlay->width + direction.end.x;

		vidbuf_overlay->ycbcr.y[index+1] = 0;
		vidbuf_overlay->ycbcr.cb[(index+1)/2] = 0;
		vidbuf_overlay->ycbcr.cr[(index+1)/2] = 255;
		vidbuf_overlay->ycbcr.y[index] = 0;
		vidbuf_overlay->ycbcr.cb[index/2] = 0;
		vidbuf_overlay->ycbcr.cr[index/2] = 125;
		vidbuf_overlay->ycbcr.y[index-1] = 0;
		vidbuf_overlay->ycbcr.cb[(index-1)/2] = 0;
		vidbuf_overlay->ycbcr.cr[(index-1)/2] = 255;	
		
		//기울기를 구해보자

		if( direction.start.y - direction.end.y == 0 ) 
		{
			way = (float)(direction.start.x - direction.end.x)/0.001f;
		}
		else 
		{
			way = (float)(direction.start.y - direction.end.y)/(float)(direction.start.x - direction.end.x);
		}
		//printf("[start: %d, %d] ", direction.start.x, direction.start.y);
		//printf("[end: %d, %d] ", direction.end.x, direction.end.y);
		//printf("way= %f\n", way);		
	}

	return way;
}
