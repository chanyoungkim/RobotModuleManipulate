#include "vectorize.h"

int vectorize(struct pxa_video_buf* buf, vector* direction) {

	//vals
	int len;
	int countx, county;
	int index, cx, cy;
	unsigned char y, cb, cr;
	int precommand=5;
	int finded;
	int command;
	float way;

		//start find vector
		finded = 0;
		//init vector
		direction->start.x = 1000;
		direction->start.y = 1000;
		direction->end.x = 1000;
		direction->end.y = 1000;
		//in 3/1 of img (320, 80) -> (\_/ h40 b40 t85 )
		//have to remove edge vals  and think about cross
		
		//wait for done
		//read(mrc3024, &command, sizeof(command));

		//right bottom to left bottom
		countx = 0;
		county = 0;
		for( cx = 2 ; cx < 319 ; cx++ ) {
			for( cy = 2 ; cy < 12 ; cy++ ) {
				//cordnate to index
				index = cy * buf->width + cx;

				//set ycbcr val
				y = buf->ycbcr.y[index];
				cb = buf->ycbcr.cb[index/2];
				cr = buf->ycbcr.cr[index/2];

				//if blue
				if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
					county++;
				}
			}

			if( county > 4 ) {
				direction->start.x = cx;
				direction->start.y = 6;
				//printf("%d\n", direction->start.x);
				finded = 1;
				break;
			}
		}
		//right bottom to right top
		if( direction->start.x == 1000 && direction->start.y == 1000 ) {
			countx = 0;
			county = 0;
			for( cy = 2 ; cy <= 50 ; cy++ ) {
				for( cx = 2 ; cx < 12 ; cx++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];

					//if blue
					if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
						countx++;
					}
				}

				if( countx > 4 ) {
					direction->start.x = 6;
					direction->start.y = cy;
					//printf("%d\n", direction->start.x);
					finded = 2;
					break;
				}
			}
		}
		//left bottom to left top
		if( direction->start.x == 1000 && direction->start.y == 1000 ) {
			countx = 0;
			county = 0;
			for( cy = 50 ; cy > 1 ; cy-- ) {
				for( cx = 308 ; cx < 318 ; cx++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];

					//if blue
					if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
						countx++;
					}
				}

				if( countx > 4 ) {
					direction->start.x = 313;
					direction->start.y = cy;
					//printf("%d\n", direction->start.x);
					finded = 3;
					break;
				}
			}
		}
		//right top to left top
		if( direction->start.x == 1000 && direction->start.y == 1000 ) {
			countx = 0;
			county = 0;
			for( cx = 2 ; cx < 319 ; cx++ ) {
				for( cy = 41 ; cy <= 50 ; cy++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];

					//if blue
					if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
						county++;
					}
				}

				if( county > 4 ) {
					direction->start.x = cx;
					direction->start.y = 75;
					//printf("%d\n", direction->start.x);
					finded = 4;
					break;
				}
			}
		}
		//end find first porint
		//left top to right top
		countx = 0;
		county = 0;
		for( cx = 318 ; cx > 1 ; cx-- ) {
			for( cy = 41 ; cy <= 50 ; cy++ ) {
				//cordnate to index
				index = cy * buf->width + cx;

				//set ycbcr val
				y = buf->ycbcr.y[index];
				cb = buf->ycbcr.cb[index/2];
				cr = buf->ycbcr.cr[index/2];

				//if blue
				if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
					county++;
				}
			}

			if( county > 4 ) {
				direction->end.x = cx;
				direction->end.y = 75;
				//printf("%d\n", direction->start.x);
				finded += 10;
				break;
			}
		}
		//left top to left bottom
		if( direction->end.x == 1000 && direction->end.y == 1000 ) {
			countx = 0;
			county = 0;
			for( cy = 50 ; cy > 1 ; cy-- ) {
				for( cx = 308 ; cx < 318 ; cx++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];
					//if blue
					if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
						countx++;
					}
				}

				if( countx > 4 ) {
					direction->end.x = 313;
					direction->end.y = cy;
					//printf("%d\n", direction->start.x);
					finded += 20;
					break;
				}
			}
		}
		//right top to right bot
		if( direction->end.x == 1000 && direction->end.y == 1000 ) {
			countx = 0;
			county = 0;
			for( cy = 50 ; cy > 1 ; cy-- ) {
				for( cx = 2 ; cx < 12 ; cx++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];

					//if blue
					if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
						countx++;
					}
				}

				if( countx > 4 ) {
					direction->end.x = 6;
					direction->end.y = cy;
					//printf("%d\n", direction->start.x);
					finded += 30;
					break;
				}
			}
		}
		//left bottom to right bottom
		if( direction->end.x == 1000 && direction->end.y == 1000 ) {
			countx = 0;
			county = 0;
			for( cx = 318 ; cx > 1 ; cx-- ) {
				for( cy = 2 ; cy < 12 ; cy++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];

					//if blue
					if( 60 < cb - cr && 140 < cb  && cr < 120  ) {
						county++;
					}
				}

				if( county > 4 ) {
					direction->end.x = cx;
					direction->end.y = 6;
					//printf("%d\n", direction->start.x);
					finded += 40;
					break;
				}
			}
		}
		//end second point

		//when not found blue line
		if( finded/10 == 0 || finded%10 == 0 ) {
			//command = 26;
			
			countx = 0;
			county = 0;
			for( cx = 318 ; cx > 1 ; cx-- ) {
				for( cy = 2 ; cy < 50 ; cy++ ) {
					//cordnate to index
					index = cy * buf->width + cx;

					//set ycbcr val
					y = buf->ycbcr.y[index];
					cb = buf->ycbcr.cb[index/2];
					cr = buf->ycbcr.cr[index/2];

					//if red
					if( 60 < cr - cb && cr > 180 && cb < 140 && 60 < y ) {
						countx++;
					}
					//if yellow
					if( 60 < cr - cb && cb < 80  && cr < 160 && 60 < y ) {
						county++;
					}
				}
			}
			
			//red and yellow both not found
			if ( countx < 15 && county < 15 ) {
				//what should i do......
				//most of dual not found
				//out of red side
				//so rutn left
				command = 4;
				//this is wrong.........
			}
			//yellow > red turn right
			else if ( county > countx ) {
				command = 6;
			}
			//red > yellow turn left
			else {
				command = 4;
			}
		}//end if not found blue line
		else {
			//printf("\n%3d %3d %3d %3d", 
			//			direction->start.x, direction->start.y,
			//			direction->end.x, direction->end.y);

			//pixel cordinator to map cordinator
			//start point
			if( finded%10 == 1 ) {
				direction->start.x = 145 - direction->start.x;
				//direction->start.x = 160 - direction->start.x;
				direction->start.y = 0;
			}
			else if( finded%10 == 2 ) {
				direction->start.x = 145 + (direction->start.y*4/3);
				//direction->start.x = 160 + (direction->start.y*4/3);
				//direction->start.y = direction->start.y;
			}
			else if( finded%10 == 3 ) {
				direction->start.x = -175 - (direction->start.y*4/3);
				//direction->start.x = -160 - (direction->start.y*4/3);
				//direction->start.y = direction->start.y;
			}
			else if( finded%10 == 4 ) {
				direction->start.x = (145 - direction->end.x)*3/2;
				//direction->start.x = (160 - direction->end.x)*3/2;
				direction->start.y = 50;
			}
			//end point
			if( finded/10 == 1 ) {
				direction->end.x = (160 - direction->end.x)*3/2;
				//direction->start.x = (160 - direction->end.x)*3/2;
				direction->end.y = 50;
			}
			else if( finded/10 == 2 ) {
				direction->end.x = -160 - (direction->start.y*4/3);
				//direction->start.x = -160 - direction->start.y;
			}
			else if( finded/10 == 3 ) {
				direction->end.x = 160 + (direction->start.y*4/3);
				//direction->start.x = 160 + direction->start.y;
			}
			else if( finded/10 == 4 ) {
				direction->end.x = 160 - direction->end.x;
				direction->end.y = 0;
			}
			//printf("\n%3d %3d %3d %3d", direction->start.x, direction->start.y,
			//			direction->end.x, direction->end.y);

			//prevent nan or infi
			if( direction->start.y - direction->end.y == 0 ) {
				way = (float)(direction->start.x - direction->end.x)/0.001f;
			}
			else {
				way = (float)(direction->start.x - direction->end.x)/(float)(direction->start.y - direction->end.y);
			}

			//printf("\n_way=%8f_", way);

			//set command move way of vector
			//go front
			if(100.0f<way || -100.0f>way)
				command = precommand;
			else if( -1.0f <= way && way <= 1.0f ) {
				command = 5;
			}
			//go smooth left
			else if( -2.0f <= way && way < -1.0f ) {
				command = 1;
			}
			//go left
			else if( -8.0f <= way && way < -2.0f ) {
				command = 4;
			}
			//go strong left
			else if( way < -8.0f ) {
				//command = 7;
				command = 4;
			}
			//go smoth right
			else if( 1.0f < way && way <= 2.0f ) {
				command = 3;
			}
			//go right
			else if(  2.0f < way && way <= 8.0f ) {
				command = 6;
			}
			//go strong right
			else if(  8.0f < way ) {
				//command = 9;
				command = 6;
			}
			//WTF????!!!!
			else {
				command = 26;
			}
		}

		precommand=command;
		 
	return command;
}
