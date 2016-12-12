#ifndef BULLET_H
#define BULLET_H

#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>
#include "collision.h"


typedef struct bullet
{
	grSprite * sprite;
	vrRigidBody * body;
	vrWorld * world;
	vrBOOL hit;
	vrVec2 initial_pos;
} bullet;


bullet * bullet_alloc();
bullet * bullet_init(bullet * bullet, vrVec2 pos, vrWorld * world);

void bullet_update(bullet * bullet, vrFloat dt);
void bullet_render(bullet * bullet, grRenderer * renderer);

void bullet_destroy(bullet * bullet);

#endif