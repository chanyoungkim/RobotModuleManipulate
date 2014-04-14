


#ifndef _VARIABLE_H_
#define _VARIABLE_H_


static float objX[6]={-1, 0, 80,60,105,20};
static float objY[6]={-1000, 0, 60,100,130,230};
static int objsX[15];
static int objsY[15];



//static float tanVal[6]={0.0f, 0.27f, 0.58f, 1.0f, 1.73f, 3.73f};
static int findflag=0;
static int command=0;
static int compMove=0;
static int state=2;
static int count=0;

#define VIDEOBUF_COUNT  3
#define STILLBUF_COUNT  2

#define CAM_STATUS_INIT     0
#define CAM_STATUS_READY    1
#define CAM_STATUS_BUSY     2

//for 3axis
#define MMA_SLEEP_MODE_ON       0x1001
#define MMA_SLEEP_MODE_OFF      0x1002
#define MMA_VIN_OFF             0x1003
#define MMA_VIN_ON              0x1004
#define MMA_SENS_15G            0x1005  // 800mVg
#define MMA_SENS_20G            0x1006  // 600mVg
#define MMA_SENS_40G            0x1007  // 300mVg
#define MMA_SENS_60G            0x1008  // 200mVg


static int cam, as, adc;
static unsigned short rxbuf[3];

void acopy(int x[], int y[]);

void sighandler(int signo);
void *checkdist(void);
void *backBoard(void);
void *findObj(void);

#endif
