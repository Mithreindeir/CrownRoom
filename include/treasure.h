#ifndef TREASURE_H
#define TREASURE_H

#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>
#include "collision.h"


typedef struct treasure
{
	grSprite * sprite;
	vrRigidBody * body;
	vrWorld * world;
	int found;
	vrVec2 initial_pos;
} treasure;


treasure * treasure_alloc();
treasure * treasure_init(treasure * treasure, vrVec2 pos, vrWorld * world);

void treasure_update(treasure * treasure, vrFloat dt);
void treasure_render(treasure * treasure, grRenderer * renderer);

void treasure_destroy(treasure * treasure);

#endif