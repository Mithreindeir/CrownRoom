#ifndef SHOOTER_H
#define SHOOTER_H

#include <velocityraptor\vrArray.h>
#include "bullet.h"
#include "enemy_container.h"
#include "player.h"

typedef struct shooter
{
	vrArray * bullets;
	vrWorld * world;
	vrVec2 pos;
	grSprite * sprite;
	player * pl;
	vrFloat dart_timer;
	int facing;
} shooter;


shooter * shooter_alloc();
shooter * shooter_init(shooter * shooter, vrWorld * world, vrVec2 pos, player * player, int facing);

void shooter_add(shooter * shooter, vrVec2 dir);
void shooter_update(enemy_container * shooterc, vrFloat dt);
void shooter_render(enemy_container * shooterc, grRenderer * renderer);

void shooter_destroy(enemy_container * shooterc);
#endif
