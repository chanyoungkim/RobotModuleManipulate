#include "labeling.h"

const ColorBoundary blue_B = {190, 250, 40, 100, 40, 100};
const ColorBoundary red_B = {345, 15, 45, 100, 56, 100};
const ColorBoundary yellow_B = {50, 76, 50, 100, 81, 100};
const ColorBoundary green_B = {160, 167, 90, 100, 71, 100};
const int BM_SIZE = 5;
const int BM_MID = 2;
/*
void pre() {
	int i = 0;
	for( i = 0 ; i < 256 ; i++ ) {
		table_298[i] = 298*i + 128;
		table_409[i] = 409*(i - 128);
		table_100[i] = 100*(i - 128);
		table_208[i] = 208*(i - 128);
		table_516[i] = 516*(i - 128);
	}
}
*/
int checkBufBoundary(struct combinedImg* buf, int x, int y){

    return (x >= 0) && (x < buf->width) && (y >= 0) && (y < buf->height);

}

int noiseFiltering(struct combinedImg* buf, int x, int y, ColorBoundary color_B) {
    HSV val;
    int cnt = 0;
    int i, j;
    int s, t;
    for (i = 0; i < BM_SIZE; ++i){
        for (j = 0; j < BM_SIZE; ++j){
            s = x - BM_MID + i;
            t = y - BM_MID + j;
            if (checkBufBoundary(buf, s, t)){
                val = ycbcr2HSV(buf, s, t);
                if (isColor(val, color_B)){
                    cnt++;
                }
            }
        }
    }
    return cnt;
}

HSV ycbcr2HSV(struct combinedImg* buf,int x, int y){
	int r, g, b;
	float h, s, v;

	ycbcr2rgb(buf, x, y, &r, &g, &b);	// ycbcr422 -> rgb
	RGBtoHSV(r, g, b, &h, &s, &v);		// rgb -> HSV

	HSV hsv = {h, s, v};
	return hsv;
}
void ycbcr2rgb(struct combinedImg* buf, int cx, int cy, int* r, int* g, int* b){

	int y, cb, cr;
	int index = cy * buf->width + cx;
	*r = *g = *b = 0;
	
	// set ycbcr val
	y = buf->ycbcr.y[index];
	cb = buf->ycbcr.cb[index/2];
	cr = buf->ycbcr.cr[index/2];
	//ycbcr to rgb fomula
	*r = min2( max2( ((y-16))*255.0/219.0 + 1.402*((cr-128)*255.0)/224.0 + 0.5 ,  0 ) ,  255);
	*g = min2( max2( ((y-16))*255.0/219.0 - 0.344*((cb-128)*255.0)/224.0 - 0.714*((cr-128)*255.0)/224.0 + 0.5 ,  0 ) ,  255);
	*b = min2( max2( ((y-16))*255.0/219.0 + 1.772*((cb-128)*255.0)/224.0 + 0.5 , 0 ) ,  255);

}

void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;
	
	min = min3(r, g, b);
	max = max3(r, g, b);
	*v = max; // v
	delta = max - min;

	if(delta == 0){ 
		*s = 0;
		*h = 0;
		*v = *v / 255.0;
		return;
	}
	if( max != 0 )
		*s = delta / max; // s
	else {
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta; // between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta; // between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta; // between magenta & cyan
	*h *= 60; // degrees
	if( *h < 0 )
		*h += 360;

	*s *= 100;
	*v = *v / 255 * 100;
}

int isColor(HSV hsv, ColorBoundary color_B){
    if(color_B.hmax < color_B.hmin){
    // 색이 Red 일 경우
        if(hsv.h >= color_B.hmin && hsv.h <= 360 && hsv.s >= color_B.smin && hsv.s <= color_B.smax && hsv.v >= color_B.vmin && hsv.v <= color_B.vmax)
            return _TRUE_;
        else if(hsv.h >= 0 && hsv.h <= color_B.hmax &&
            hsv.s >= color_B.smin && hsv.s <= color_B.smax &&
            hsv.v >= color_B.vmin && hsv.v <= color_B.vmax)
            return _TRUE_;
        else
            return _FALSE_;
    }
    else{    // 그 외 보통 경우
        if(hsv.h >= color_B.hmin && hsv.h <= color_B.hmax && hsv.s >= color_B.smin && hsv.s <= color_B.smax && hsv.v >= color_B.vmin && hsv.v <= color_B.vmax)
            return _TRUE_;
        else
            return _FALSE_;
    }
}

int verticalProject(struct combinedImg* buf, int* vproj) {
	//int r, g, b;
	//float h, s, i, angle;
	//int y,cb,cr;
	int cx, cy;
	int sumy;
	int county;
	int index;
	int noisecount;	

	for( cx = 0 ; cx < buf->width ; cx++ ) {
		sumy = 0;
		county = 0;
		for( cy = 0 ; cy < buf->height ; cy++ ) {
		/*			
			index = cy * buf->width + cx;
			
			//set ycbcr val
			y = buf->ycbcr.y[index];
			cb = buf->ycbcr.cb[index/2];
			cr = buf->ycbcr.cr[index/2];

			r = (table_298[y]+table_409[cr]+128)>>8;
			g = (table_298[y]-table_100[cb]-table_208[cr]+128)>>8;
			b = (table_298[y]+table_516[cb]+128)>>8;

			i = (r+g+b)/3.0f;

			s = 1.0f-min(r, g, b)/i;
			//angle = (r-0.5f*g-0.5f*b)/(float)sqrt((r-g)*(r-g)+(r-b)*(g-b));
			h = (float)acos((r-0.5f*g-0.5f*b)/(float)sqrt((r-g)*(r-g)+(r-b)*(g-b)));

			h *= 57.29577951f;
			
			if(b>g)
				h = 360.0f - h;

			//printf("%f ", s);
			//if red
			if( h < 15 || 345 < h && 0.4 < s && s < 0.9 && 50 < i && i < 140 ) {
				sumy += cy;
				county++;
			}
			*/
						
			HSV hsv = ycbcr2HSV(buf, cx, cy);
			//if coke
						
			
			if(isColor(hsv, red_B)){
				sumy += cy;
				county++;
				printf("milk found\n");
			}
			
			// if milk
			if(isColor(hsv, green_B)){
				sumy += cy;
				county++;
				printf("milk found\n");
			}
			// if plastic container
			if(isColor(hsv, red_B) && (noiseFiltering(buf, cx, cy, red_B)<6)){
				sumy += cy;
				county++;
				printf("plastic found\n");
			
			}
					
		}
		//size filter
		if( county < 32 ){
			sumy = 0;
		}
		else {
			sumy /= county;
		}
		vproj[cx] = sumy;
	}
	//printf("noisecount : %d\n",noisecount);
	return 0;
}

int setBoundary(struct combinedImg* buf, int* vproj, int* bound) {
	int on_obj;
	int cx, bi;
	
	//init vals
	for( bi = 0 ; bi < 30 ; bi++ ) {
		bound[bi] = 1000;
	}
	on_obj = 0;
	bi = 0;

	for( cx = 0 ; cx < buf->width ; cx++ ) {
		//first something exist
		if( vproj[cx] != 0 && !on_obj ) {
			on_obj = 1;
			bound[bi*2] = cx;

			printf("f%d ", cx);
		}
		//on_obj~
		else if( vproj[cx] != 0 && on_obj ) {
			//todo : find detect change obj
		}
		//nothing but was exist
		else if( on_obj ) {
			on_obj = 0;
			bound[bi*2+1] = cx;
			bi++;

			printf("e%d  ", cx);
		}

	}
	
	printf("\n", cx);
	return 0;
}


int getYVals(struct combinedImg* buf, int* vproj, int* bound, int* heights) {
	int bi, cy, mid;
	int index;
	int ty, tcb, tcr;
	int y, cb, cr;
	int endflag;	

	for( bi = 0 ; bi < 30 ; bi++ )
		heights[bi] = 1000;
	//
	//extreme left or right
	//
	for( bi = 0 ; bi < 30 ; bi++ ) {
		endflag = 0;
		if( bound[bi] == 1000 || bound[bi + 1] == 1000 ) {
			break;
		}
		mid = (bound[bi] + bound[bi + 1])/2;
		//no object in bound
		if ( vproj[mid] == 0 ) {
			continue;
		}
		//cordnate to index
		index = vproj[mid] * buf->width + mid;
		//init tycbcr vals
		ty = buf->ycbcr.y[index];
		tcb = buf->ycbcr.cb[index/2];
		tcr = buf->ycbcr.cr[index/2];
		//move down
		for( cy = vproj[mid] - 3 ; cy > 0 ; cy-- ) {
			//cordnate to index
			index = cy * buf->width + mid;
			
			//set ycbcr val
			y = buf->ycbcr.y[index];
			cb = buf->ycbcr.cb[index/2];
			cr = buf->ycbcr.cr[index/2];

			//if color changed
			if( y < ty - 20 || ty + 20 < y 
			|| cb < tcb - 10 || tcb + 10 < cb
			|| cr < tcr - 10 || tcr + 10 < cr ) {
				heights[bi] = cy;
				printf("%2d[%3d,", bi/2, cy);
	
				endflag = 1;
				
				break;
			}

			//reset tcbcrvals
			ty = buf->ycbcr.y[index];
			tcb = buf->ycbcr.cb[index/2];
			tcr = buf->ycbcr.cr[index/2];
		}	

		if( endflag < 1 ) {
			heights[bi] = 0;
			printf("%2d[  0,", bi);
		}


		//init tycbcr vals
		index = (vproj[mid] + 2) * buf->width + mid;
		ty = buf->ycbcr.y[index];
		tcb = buf->ycbcr.cb[index/2];
		tcr = buf->ycbcr.cr[index/2];	

		//move up
		for( cy = vproj[mid] + 3 ; cy < buf->height ; cy++ ) {
			//cordnate to index
			index = cy * buf->width + mid;
			
			//set ycbcr val
			y = buf->ycbcr.y[index];
			cb = buf->ycbcr.cb[index/2];
			cr = buf->ycbcr.cr[index/2];

			//if color changed
			if( y < ty - 20 || ty + 20 < y 
			|| cb < tcb - 10 || tcb + 10 < cb
			|| cr < tcr - 10 || tcr + 10 < cr ) {
				heights[bi+1] = cy;
				printf("%3d]__", cy);
	
				endflag = 2;
				
				break;
			}

			//reset tcbcrvals
			ty = buf->ycbcr.y[index];
			tcb = buf->ycbcr.cb[index/2];
			tcr = buf->ycbcr.cr[index/2];
		}

		if( endflag < 2 ) {
			heights[bi+1] = cy;
			printf("240]__");
		}


	}
	printf("\n");

	return 0;
}

int drawProject(struct combinedImg* buf, int* vproj) {
	int cx, cy;
	int index;

	for( cx = 0 ; cx < buf->width ; cx++ ) {
		//cordnate to index
		index = vproj[cx] * buf->width + cx;
			
		buf->ycbcr.y[index] = 128;
		buf->ycbcr.cb[index/2] = 16;
		buf->ycbcr.cr[index/2] = 16;

		if( vproj[cx] > 5 ) {
			index = (vproj[cx]-1) * buf->width + cx;
			buf->ycbcr.y[index] = 128;
			buf->ycbcr.cb[index/2] = 16;
			buf->ycbcr.cr[index/2] = 16;

			index = (vproj[cx]-2) * buf->width + cx;
			buf->ycbcr.y[index] = 128;
			buf->ycbcr.cb[index/2] = 16;
			buf->ycbcr.cr[index/2] = 16;
			index = (vproj[cx]-3) * buf->width + cx;
			buf->ycbcr.y[index] = 128;
			buf->ycbcr.cb[index/2] = 16;
			buf->ycbcr.cr[index/2] = 16;
			index = (vproj[cx]-4) * buf->width + cx;
			buf->ycbcr.y[index] = 128;
			buf->ycbcr.cb[index/2] = 16;
			buf->ycbcr.cr[index/2] = 16;
		}

	}

	return 0;
}

int drawBoundary(struct combinedImg* buf, int* bound) {
	int bi, cy;
	int index;

	for( bi = 0 ; bi < 30 ; bi++ ) {
		if( bound[bi] == 1000 ) {
			break;
		}
		for( cy = 0 ; cy < buf->height ; cy++ ) {
			//cordnate to index
			index = cy * buf->width + bound[bi];
			
			buf->ycbcr.y[index] = 128;
			buf->ycbcr.cb[index/2] = 0;
			buf->ycbcr.cr[index/2] = 255;
			
			if( index > 3 ) {
				buf->ycbcr.y[index-1] = 128;
				buf->ycbcr.cb[index/2-1] = 0;
				buf->ycbcr.cr[index/2-1] = 255;
				buf->ycbcr.y[index-2] = 128;
				buf->ycbcr.cb[index/2-2] = 0;
				buf->ycbcr.cr[index/2-2] = 255;
			}
		}
	}

	return 0;
}
