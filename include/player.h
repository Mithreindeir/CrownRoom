#ifndef PLAYER_H
#define PLAYER_H

#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>
#include "collision.h"
#include "treasure.h"
#include "sound.h"


#define SCALE 1
#define JUMP_TIME 1
#define MAX_JUMP_HEIGHT (4*(16*4) + 4)
#define MIN_JUMP_HEIGHT (1*(16*4))

#define GRAVITY (2*MAX_JUMP_HEIGHT)/pow(JUMP_TIME, 2)
#define JUMP_SPEED (sqrt(2*GRAVITY*MAX_JUMP_HEIGHT))
#define EARLY_TERMINATION (sqrt(pow(JUMP_SPEED, 2) + 2 * -GRAVITY * (MAX_JUMP_HEIGHT - MIN_JUMP_HEIGHT)))

typedef struct player
{
	vrRigidBody * body;
	grSprite * sprite;
	vrWorld * world;
	vrBOOL flip;
	vrBOOL on_ground;
	vrBOOL on_wall;
	int rw; //on right wall if true, left is false
	int has_crown;
	int rotate; 
	treasure * tr;
	vrJoint * rope;
	int has_rope;
	sound * jump;
	sound * hit;
	sound * crown_sound;
	sound * rope_sound;
} player;

player * player_alloc();
player * player_init(player * player, vrVec2 pos, vrWorld * world, treasure * treasure);

void player_update(player * player, vrFloat dt);
void player_render(player * player, grRenderer * renderer);

void player_destroy(player * player);

#endif