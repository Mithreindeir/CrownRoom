#pragma once

#ifndef COLLISION_H
#define COLLISION_H

typedef enum masks
{
	WALL = 1,
	PLAYER = 2,
	ENEMY = 4,
	PLAYER_BULLET = 16,
	ENEMY_BULLET = 32,
	EFFECT = 64,
	TREASURE = 128,
	DANGEROUS = 256
} masks;

#endif