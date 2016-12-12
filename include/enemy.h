#ifndef ENEMY_H
#define ENEMY_H


#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>
#include "collision.h"
#include "player.h"
#include "enemy_container.h"


#define SCALE 1
#define JUMP_TIME 1
#define MAX_JUMP_HEIGHT (4*(16*4) + 4)
#define MIN_JUMP_HEIGHT (1*(16*4))

#define GRAVITY (2*MAX_JUMP_HEIGHT)/pow(JUMP_TIME, 2)
#define JUMP_SPEED (sqrt(2*GRAVITY*MAX_JUMP_HEIGHT))
#define EARLY_TERMINATION (sqrt(pow(JUMP_SPEED, 2) + 2 * -GRAVITY * (MAX_JUMP_HEIGHT - MIN_JUMP_HEIGHT)))

typedef struct enemy
{
	vrRigidBody * body;
	grSprite * sprite;
	vrWorld * world;
	vrBOOL flip;
	vrBOOL on_ground;
	vrBOOL on_wall;
	int rw; //on right wall if true, left is false
	player * mp;
} enemy;


enemy * enemy_alloc();
enemy * enemy_init(enemy * enemy, vrVec2 pos, vrWorld * world, player * player);

void enemy_update(enemy_container * enemyc, vrFloat dt);
void enemy_render(enemy_container * enemyc, grRenderer * renderer);

void enemy_destroy(enemy_container * enemyc);

#endif
