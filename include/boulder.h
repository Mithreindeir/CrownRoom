#ifndef BOULDER_H
#define BOULDER_H


#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>
#include "collision.h"
#include "player.h"
#include "enemy_container.h"

typedef struct boulder
{
	vrRigidBody * body;
	grSprite * sprite;
	vrWorld * world;
	vrBOOL flip;
	vrFloat ft;
	vrBOOL on_ground;
	vrBOOL on_wall;
	int rw; //on right wall if true, left is false
	player * mp;
} boulder;


boulder * boulder_alloc();
boulder * boulder_init(boulder * boulder, vrVec2 pos, vrWorld * world, player * player);

void boulder_update(enemy_container * boulderc, vrFloat dt);
void boulder_render(enemy_container * boulderc, grRenderer * renderer);

void boulder_destroy(enemy_container * boulderc);

#endif
