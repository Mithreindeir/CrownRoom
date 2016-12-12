#ifndef HEADER_EDITOR
#define HEADER_EDITOR

#include <velocityraptor/vrWorld.h>

typedef enum tile_type
{
	LONE = 0,
	LONE_UP = 1,
	LONE_LEFT = 2,
	LONE_DOWN = 4,
	LONE_RIGHT = 8,
	CORNER_UP_LEFT = 12,
	CORNER_UP_RIGHT = 6,
	CORNER_DOWN_LEFT = 9,
	CORNER_DOWN_RIGHT = 3,
	MIDDLE_UP = 14,
	MIDDLE_LEFT = 13,
	MIDDLE_DOWN = 11,
	MIDDLE_RIGHT = 7,
	DOUBLE_LR = 10,
	DOUBLE_TB = 5,
	MIDDLE = 15
} tile_type;

int get_tile_index(int top, int bottom, int left, int right);
void create_walls_from_grid(int* ids, int width, int height, vrWorld* world);

#endif#pragma once
