#ifndef _COLOR_BOUNDARY_H_
#define _COLOR_BOUNDARY_H_

typedef struct {
	int hmin;
	int hmax;
	int smin;
	int smax;
	int vmin;
	int vmax;
}ColorBoundary;

const ColorBoundary blue_B = {190, 250, 40, 100, 40, 100};

const ColorBoundary red_B = {345, 15, 45, 100, 56, 100};

const ColorBoundary yellow_B = {50, 76, 50, 100, 81, 100};

const ColorBoundary green_B = {160, 167, 90, 100, 71, 100};

#endif;
