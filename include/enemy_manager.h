#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "boulder.h"
#include <velocityraptor\vrArray.h>
#include <velocityraptor\vrJoint.h>
#include <velocityraptor\vrDistanceJoint.h>
#include "shooter.h"

typedef struct enemy_manager
{
	vrArray * enemies;
	player * pl;
	vrWorld * world;
} enemy_manager;


enemy_manager * enemy_manager_alloc();
enemy_manager * enemy_manager_init(enemy_manager * enemy_manager, vrWorld * world, player * p);

void enemy_manager_add(enemy_manager * manager, vrVec2 pos, int id);
void enemy_manager_update(enemy_manager * enemy_manager, vrFloat dt);
void enemy_manager_render(enemy_manager * enemy_manager, grRenderer * renderer);

void enemy_manager_destroy(enemy_manager * enemy_manager);
#endif