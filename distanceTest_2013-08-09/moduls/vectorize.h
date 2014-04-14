#ifndef	_VECTORIZE_H_
#define	_VECTORIZE_H_

#include <math.h>

#include <pxa_lib.h>
#include <pxa_camera_zl.h>

typedef struct point{
	int x;
	int y;
}point;

typedef struct vector{
	point start;
	point end;
}vector;

int vectorize(struct pxa_video_buf* buf, vector* v);

#endif
