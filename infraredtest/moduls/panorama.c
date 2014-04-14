#include "panorama.h"

int makeImg (struct combinedImg* cImg, int imageFlag, int isRightimage, struct pxa_video_buf* buf)
{
	int cx, cy;
	int index;
	int val;
	if( imageFlag == 0 ) //왼쪽
	{		
		if(isRightimage) //오른쪽이면서 자를거야
		{
			for( cy= 0 ; cy < buf->height ; cy++ )
			{
				for( cx = 0 ; cx < buf->width ; cx++ ) 
				{
					index = cy * buf->width + cx;
					val = cy + (3.48741444f) * cx; // tan74
					if( val < (320) ) //761.888
					{ // 320 = tan 74 * 0 + val
						/*
						cImg->ycbcr.y[index + buf->width*cy + buf->width ] = 255;
						cImg->ycbcr.cb[index/2 + buf->width*cy/2 + buf->width/2 ] = 128;
						cImg->ycbcr.cr[index/2 + buf->width*cy/2 + buf->width/2 ] = 128;*/
						
		
					}
					else //원래사진
					{
						cImg->ycbcr.y[index + buf->width*cy + buf->width - (int)((240-cy)/3.48741444) - 30] = buf->ycbcr.y[index]; // tan74	
						cImg->ycbcr.cb[index/2 + buf->width*cy/2 + buf->width/2 - (int)((240-cy)/6.97482) -15] = buf->ycbcr.cb[index/2]; // tan74 /2
						cImg->ycbcr.cr[index/2 + buf->width*cy/2 + buf->width/2 - (int)((240-cy)/6.97482) -15] = buf->ycbcr.cr[index/2];
					}
				}
			}		
		}
		else
		{
			for( cy= 0 ; cy < buf->height ; cy++ )
			{
				for( cx = 0 ; cx < buf->width ; cx++ ) 
				{
					index = cy * buf->width + cx;
		
					//todo : 인댁스 설
					cImg->ycbcr.y[index + buf->width*cy + buf->width] = buf->ycbcr.y[index];
					cImg->ycbcr.cb[index/2 + buf->width*cy/2 + buf->width/2] = buf->ycbcr.cb[index/2];
					cImg->ycbcr.cr[index/2 + buf->width*cy/2 +  buf->width/2] = buf->ycbcr.cr[index/2];

				}
			}
		}


	}
	else //오른쪽
	{
		if(isRightimage) //오른쪽이면서 자를거야
		{
			for( cy= 0 ; cy < buf->height ; cy++ )
			{
				for( cx = 0 ; cx < buf->width ; cx++ ) 
				{
					index = cy * buf->width + cx;
					val = cy - (3.48741444f) * cx;//  1/tan16
					if( val < (-793.4852075f) ) //761.888
					{ // y절편 320 = 1/tan16 * 320 + val 
						/*
						cImg->ycbcr.y[index + buf->width*cy] = 255;
						cImg->ycbcr.cb[index/2 + buf->width*cy/2] = 128;
						cImg->ycbcr.cr[index/2 + buf->width*cy/2] = 128;
						*/
		
					}
					else //원래사진
					{
						cImg->ycbcr.y[index + buf->width*cy + (int)(0.286745385*(240-cy))+24] = buf->ycbcr.y[index];	 //tan16 * (240 - cy)
						cImg->ycbcr.cb[index/2 + buf->width*cy/2 + (int)(0.14337269287*(240-cy))+12] = buf->ycbcr.cb[index/2]; // /2
						cImg->ycbcr.cr[index/2 + buf->width*cy/2 + (int)(0.14337269287*(240-cy))+12] = buf->ycbcr.cr[index/2];
					}
				}
			}		
		}
		else
		{
			for( cy= 0 ; cy < buf->height ; cy++ )
			{
				for( cx = 0 ; cx < buf->width ; cx++ ) 
				{
					index = cy * buf->width + cx;
		
					cImg->ycbcr.y[index + buf->width*cy] = buf->ycbcr.y[index];
					cImg->ycbcr.cb[index/2 + buf->width*cy/2] = buf->ycbcr.cb[index/2];
					cImg->ycbcr.cr[index/2 + buf->width*cy/2] = buf->ycbcr.cr[index/2];
				}
			}
		}
		
	}

	return 0;
}

int integrateImg (struct combinedImg* cImg, struct pxa_video_buf* buf)
{
	int cx, cy, index;
	for( cy= 0 ; cy < cImg->height ; cy++ )
	{
		for( cx = 0 ; cx < cImg->width ; cx=cx+2 ) 
		{
			index = cy * cImg->width + cx;
	
			buf->ycbcr.y[index/2] = cImg->ycbcr.y[index];
			buf->ycbcr.cb[index/4] = cImg->ycbcr.cb[index/2];
			buf->ycbcr.cr[index/4] = cImg->ycbcr.cr[index/2];
		}
	}

	return 0;
}
