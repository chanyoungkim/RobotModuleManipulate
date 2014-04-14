#include <pxa_lib.h>
#include <pxa_camera_zl.h>

typedef struct anglePoint{
	int x;
	int y;
}anglePoint;

typedef struct angleVector{
	anglePoint start;
	anglePoint end;
}angleVector;

float angleRecognition (struct pxa_video_buf* vidbuf);

