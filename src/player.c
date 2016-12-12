#include "../include/player.h"

player * player_alloc()
{
	return malloc(sizeof(player));
}

player * player_init(player * player, vrVec2 pos, vrWorld * world, treasure * treasure)
{
	player->body = vrBodyInit(vrBodyAlloc());
	player->body->center = pos;

	vrShape* shape = vrShapeInit(vrShapeAlloc());
	//shape = vrShapePolyInit(shape);
	//shape->shape = vrPolyBoxInit(shape->shape, pos.x + 5*4, pos.y + 3*4, 6 * 4, 12 * 4);
	//vrArrayPush(player->body->shape, shape);
	shape = vrShapeCircleInit(shape);
	((vrCircleShape*)shape->shape)->center = pos;
	((vrCircleShape*)shape->shape)->radius = 4 * 4;
	vrArrayPush(player->body->shape, shape);


	player ->body->bodyMaterial.invMass = 1.0 / 1.0;
	player->body->bodyMaterial.invMomentInertia = 1.0/vrMomentForCircle(4 * 4, 1);

	player->sprite = grSpriteInit(grSpriteAlloc(), "character.png", 0);
	grSpriteSetUpAnimation(player->sprite, 7, 6, 0, 0);
	player->sprite->speed = 500;
	player->sprite->size = grVec2Scale(player->sprite->size, 4.0);

	vrWorldAddBody(world, player->body);
	player->world = world;
	player->flip = vrTRUE;
	player->body->bodyMaterial.friction = 0.5;
	player->body->bodyMaterial.angularDamping = 0.4;
	player->sprite->center = 1;
	
	player->has_crown = 0;
	player->tr = treasure;

	player->body->collisionData.categoryMask = PLAYER;
	player->body->collisionData.maskBit = ENEMY | WALL | TREASURE;
	player->rotate = 0;
	player->has_rope = 0;
	player->rope = NULL;
	player->jump = sound_init(sound_alloc(), "jump.wav");
	player->hit = sound_init(sound_alloc(), "hit.wav");
	player->crown_sound = sound_init(sound_alloc(), "explosion.wav");
	player->rope_sound = sound_init(sound_alloc(), "rope.wav");


	return player;
}

void player_update(player * player, vrFloat dt)
{
	//vrPolygonShape* shape = ((vrShape*)player->body->shape->data[0])->shape;
	//vrVec2 topleft = shape->vertices[0];
	//player->sprite->pos = grV2(topleft.x - 5*4, topleft.y - 3*4);
	vrCircleShape* shape = ((vrShape*)player->body->shape->data[0])->shape;
	player->sprite->pos = grV2(shape->center.x, shape->center.y - 3*4);


	player->body->angularVelocity *= 0.95;
	if (abs(player->body->angularVelocity) < 1)
		player->body->angularVelocity = 0;
	if (abs(player->body->velocity.x) < 5)
		player->body->velocity.x = 0;

	if (player->body->velocity.x > 0)
		player->flip = vrTRUE;
	else if (player->body->velocity.x < 0)
		player->flip = vrFALSE;

	if (player->flip)
	{
		if (player->on_ground)
		{
			if (player->body->velocity.x > 0) grSpriteUpdate(player->sprite, dt, 1 + player->has_crown, 6 + player->has_crown);
			else grSpriteUpdate(player->sprite, dt, 0 + player->has_crown, 0 + player->has_crown);
		}
		
	}
	else
	{
		if (player->on_ground)
		{
			if (player->body->velocity.x < 0) grSpriteUpdate(player->sprite, dt, 8 + player->has_crown, 13 + player->has_crown);
			else grSpriteUpdate(player->sprite, dt, 7 + player->has_crown, 7 + player->has_crown);
		}
	}
	if (!player->on_ground)
	{
		if (player->on_wall)
		{
			if (player->rw)
			{
				grSpriteUpdate(player->sprite, dt, 14 + player->has_crown, 14 + player->has_crown);
			}
			else
			{
				grSpriteUpdate(player->sprite, dt, 15 + player->has_crown, 15 + player->has_crown);
			}
		}
		else
		{
			if (player->flip)
			{
				if (player->body->velocity.y < 0) grSpriteUpdate(player->sprite, dt, 16 + player->has_crown, 16 + player->has_crown);
				else grSpriteUpdate(player->sprite, dt, 17 + player->has_crown, 17 + player->has_crown);
			}
			else
			{
				if (player->body->velocity.y < 0) grSpriteUpdate(player->sprite, dt, 18 + player->has_crown, 18 + player->has_crown);
				else grSpriteUpdate(player->sprite, dt, 19 + player->has_crown, 19 + player->has_crown);
			}
		}
	}

	//
	vrBOOL og = player->on_ground;
	player->on_ground = vrFALSE;
	for (int i = 0; i < player->body->manifolds->sizeof_active; i++)
	{
		//if (p->on_ground)
		//	continue;
		vrManifold* m = player->body->manifolds->data[i];
		if (!(m->A->collisionData.categoryMask == WALL || m->B->collisionData.categoryMask == WALL))
			continue;
		if (m->A == player->body)
		{
			if ((int)(m->normal.y - 1.0) == 0)
				player->on_ground = vrTRUE;
		}
		else
		{
			if ((int)(m->normal.y + 1.0) == 0)
				player->on_ground = vrTRUE;
		}
	}
	player->on_wall = vrFALSE;
	for (int i = 0; i < player->body->manifolds->sizeof_active; i++)
	{
		//if (p->on_ground)
		//	continue;
		vrManifold* m = player->body->manifolds->data[i];
		if (!(m->A->collisionData.categoryMask == WALL || m->B->collisionData.categoryMask == WALL))
			continue;
		if (m->A == player->body)
		{
			if ((int)(m->normal.x - 1.0) == 0)
			{
				player->on_wall = vrTRUE;
				player->rw = 1;
			}
			else if ((int)(m->normal.x + 1.0) == 0)
			{
				player->on_wall = vrTRUE;
				player->rw = 0;
			}
		}
		else
		{
			if ((int)(m->normal.x + 1.0) == 0)
			{
				player->on_wall = vrTRUE;
				player->rw = 1;
			}
			else if ((int)(m->normal.x - 1.0) == 0)
			{
				player->on_wall = vrTRUE;
				player->rw = 0;
			}

		}
	}

	for (int i = 0; i < player->body->manifolds->sizeof_active; i++)
	{
		vrManifold* m = player->body->manifolds->data[i];
		if (m->A->collisionData.categoryMask == TREASURE || m->B->collisionData.categoryMask == TREASURE)
		{
			sound_play(player->crown_sound);
			player->tr->found = 1;
			player->has_crown = 21;
		}
		else if (m->A->collisionData.categoryMask == ENEMY || m->B->collisionData.categoryMask == ENEMY || m->A->collisionData.categoryMask == ENEMY_BULLET || m->B->collisionData.categoryMask == ENEMY_BULLET)
		{
			if (player->has_crown != 0)
			{
				sound_play(player->hit);
				player->tr->found = 0;
				player->has_crown = 0;
				vrBodySet(player->body, vrVect(8 * 4, 50), player->body->orientation);
				//player->rotate = 1;
				//vrBodySet(player->tr->body, player->tr->initial_pos, player->tr->body->orientation);
			}
		}
	}

}

void player_render(player * player, grRenderer * renderer)
{
	if (player->rotate)
		player->sprite->rotation = player->body->orientation;
	grSpriteRender(player->sprite, renderer);
}

void player_destroy(player * player)
{
	sound_destroy(player->jump);
	sound_destroy(player->hit);
	sound_destroy(player->crown_sound);
	sound_destroy(player->rope_sound);

	if (player->has_rope)
	{
		vrArrayErase(player->world->joints, 0);
		player->has_rope = 0;
		vrDistanceConstraintDestroy(player->rope);
	}
	vrWorldRemoveBody(player->world, player->body);
	grSpriteDestroy(player->sprite);
	free(player);
}
