#include "../include/enemy.h"

enemy * enemy_alloc()
{
	return malloc(sizeof(enemy));
}

enemy * enemy_init(enemy * enemy, vrVec2 pos, vrWorld * world, player * player)
{
	enemy->body = vrBodyInit(vrBodyAlloc());

	vrShape* shape = vrShapeInit(vrShapeAlloc());
	//shape = vrShapePolyInit(shape);
	//shape->shape = vrPolyBoxInit(shape->shape, pos.x + 5*4, pos.y + 3*4, 6 * 4, 12 * 4);
	//vrArrayPush(enemy->body->shape, shape);
	shape = vrShapeCircleInit(shape);
	((vrCircleShape*)shape->shape)->center = pos;
	((vrCircleShape*)shape->shape)->radius = 4 * 4;
	vrArrayPush(enemy->body->shape, shape);
	enemy->body->center = pos;

	enemy->body->bodyMaterial.invMass = 1.0 / 1.0;
	enemy->body->bodyMaterial.invMomentInertia = 1.0 / vrMomentForCircle(4 * 4, 1);

	enemy->sprite = grSpriteInit(grSpriteAlloc(), "enemy.png", 0);
	grSpriteSetUpAnimation(enemy->sprite, 7, 3, 0, 0);
	enemy->sprite->speed = 500;
	enemy->sprite->size = grVec2Scale(enemy->sprite->size, 4.0);

	vrWorldAddBody(world, enemy->body);
	enemy->world = world;
	enemy->flip = vrTRUE;
	enemy->body->bodyMaterial.friction = 0.5;
	enemy->body->bodyMaterial.angularDamping = 0.4;
	enemy->sprite->center = 1;
	enemy->mp = player;

	enemy->body->collisionData.categoryMask = ENEMY;
	enemy->body->collisionData.maskBit = PLAYER | WALL;

	return enemy;
}

void enemy_update(enemy_container * enemyc, vrFloat dt)
{
	enemy * enemy = enemyc->enemy;
	//vrPolygonShape* shape = ((vrShape*)enemy->body->shape->data[0])->shape;
	//vrVec2 topleft = shape->vertices[0];
	//enemy->sprite->pos = grV2(topleft.x - 5*4, topleft.y - 3*4);
	vrCircleShape* shape = ((vrShape*)enemy->body->shape->data[0])->shape;
	enemy->sprite->pos = grV2(shape->center.x, shape->center.y - 3 * 4);


	enemy->body->angularVelocity *= 0.98;
	if (abs(enemy->body->angularVelocity) < 1)
		enemy->body->angularVelocity = 0;
	if (abs(enemy->body->velocity.x) < 5)
		enemy->body->velocity.x = 0;

	if (enemy->body->velocity.x > 0)
		enemy->flip = vrTRUE;
	else if (enemy->body->velocity.x < 0)
		enemy->flip = vrFALSE;

	if (enemy->flip)
	{
		if (enemy->on_ground)
		{
			if (enemy->body->velocity.x > 0) grSpriteUpdate(enemy->sprite, dt, 1, 6);
			else grSpriteUpdate(enemy->sprite, dt, 0, 0);
		}

	}
	else
	{
		if (enemy->on_ground)
		{
			if (enemy->body->velocity.x < 0) grSpriteUpdate(enemy->sprite, dt, 8, 13);
			else grSpriteUpdate(enemy->sprite, dt, 7, 7);
		}
	}
	if (!enemy->on_ground)
	{
		if (enemy->flip)
		{
			if (enemy->body->velocity.y < 0) grSpriteUpdate(enemy->sprite, dt, 16, 16);
			else grSpriteUpdate(enemy->sprite, dt, 17, 17);
		}
		else
		{
			if (enemy->body->velocity.y < 0) grSpriteUpdate(enemy->sprite, dt, 18, 18);
			else grSpriteUpdate(enemy->sprite, dt, 19, 19);
		}
	}

	//
	vrBOOL og = enemy->on_ground;
	enemy->on_ground = vrFALSE;
	for (int i = 0; i < enemy->body->manifolds->sizeof_active; i++)
	{
		//if (p->on_ground)
		//	continue;
		vrManifold* m = enemy->body->manifolds->data[i];
		if (!(m->A->collisionData.categoryMask == WALL || m->B->collisionData.categoryMask == WALL))
			continue;
		if (m->A == enemy->body)
		{
			if ((int)(m->normal.y - 1.0) == 0)
				enemy->on_ground = vrTRUE;
		}
		else
		{
			if ((int)(m->normal.y + 1.0) == 0)
				enemy->on_ground = vrTRUE;
		}
	}
	enemy->on_wall = vrFALSE;
	for (int i = 0; i < enemy->body->manifolds->sizeof_active; i++)
	{
		//if (p->on_ground)
		//	continue;
		vrManifold* m = enemy->body->manifolds->data[i];
		if (!(m->A->collisionData.categoryMask == WALL || m->B->collisionData.categoryMask == WALL))
			continue;
		if (m->A == enemy->body)
		{
			if ((int)(m->normal.x - 1.0) == 0)
			{
				enemy->on_wall = vrTRUE;
				enemy->rw = 1;
			}
			else if ((int)(m->normal.x + 1.0) == 0)
			{
				enemy->on_wall = vrTRUE;
				enemy->rw = 0;
			}
		}
		else
		{
			if ((int)(m->normal.x + 1.0) == 0)
			{
				enemy->on_wall = vrTRUE;
				enemy->rw = 1;
			}
			else if ((int)(m->normal.x - 1.0) == 0)
			{
				enemy->on_wall = vrTRUE;
				enemy->rw = 0;
			}

		}
	}
	if (enemy->mp->has_crown != 0)
	{
		vrVec2 d = vrSub(enemy->body->center, enemy->mp->body->center);

		int r = d.x < 0;
		int l = d.x > 0;
		if ((rand() % 10) > 5)
		{
			l = 0;
			r = 0;
		}
		if (d.y > 50 && enemy->on_ground)
		{
			enemy->body->velocity.y = -JUMP_SPEED;
		}
		else if (d.y > 50 && enemy->on_wall)
		{

			if (enemy->rw)
			{
				enemy->body->velocity.y = -JUMP_SPEED / 1.5;
				enemy->body->velocity.x = -JUMP_SPEED / 2.0;
			}
			else
			{
				enemy->body->velocity.y = -JUMP_SPEED / 1.5;
				enemy->body->velocity.x = JUMP_SPEED / 2.0;
			}
		}
		if (l)
		{
			if (enemy->body->angularVelocity > -10)
				enemy->body->angularVelocity -= 5;
			else
				enemy->body->angularVelocity = -10;

			if (enemy->body->velocity.x > -350)
			{
				enemy->body->velocity.x -= 10;
			}
			else enemy->body->velocity.x = -350;
		}
		if (r)
		{
			if (enemy->body->angularVelocity < 10)
				enemy->body->angularVelocity += 5;
			else
				enemy->body->angularVelocity = 10;

			if (enemy->body->velocity.x < 350)
			{
				enemy->body->velocity.x += 10;
			}
			else enemy->body->velocity.x = 350;
		}
	}
}

void enemy_render(enemy_container * enemyc, grRenderer * renderer)
{
	enemy * enemy = enemyc->enemy;
	grSpriteRender(enemy->sprite, renderer);
}

void enemy_destroy(enemy_container * enemyc)
{
	enemy * enemy = enemyc->enemy;

	vrWorldRemoveBody(enemy->world, enemy->body);
	grSpriteDestroy(enemy->sprite);
	free(enemy);
}
