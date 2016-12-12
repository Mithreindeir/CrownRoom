#include "../include/boulder.h"

boulder * boulder_alloc()
{
	return malloc(sizeof(boulder));
}

boulder * boulder_init(boulder * boulder, vrVec2 pos, vrWorld * world, player * player)
{
	boulder->body = vrBodyInit(vrBodyAlloc());

	vrShape* shape = vrShapeInit(vrShapeAlloc());
	//shape = vrShapePolyInit(shape);
	//shape->shape = vrPolyBoxInit(shape->shape, pos.x + 5*4, pos.y + 3*4, 6 * 4, 12 * 4);
	//vrArrayPush(boulder->body->shape, shape);
	shape = vrShapeCircleInit(shape);
	((vrCircleShape*)shape->shape)->center = pos;
	((vrCircleShape*)shape->shape)->radius = 8 * 4;
	vrArrayPush(boulder->body->shape, shape);
	boulder->body->center = pos;

	boulder->body->bodyMaterial.invMass = 1.0 / 1.0;
	boulder->body->bodyMaterial.invMomentInertia = 1.0 / vrMomentForCircle(8 * 4, 1);

	boulder->sprite = grSpriteInit(grSpriteAlloc(), "boulder.png", 0);
	grSpriteSetUpAnimation(boulder->sprite, 1, 1, 0, 0);
	boulder->sprite->speed = 500;
	boulder->sprite->size = grVec2Scale(boulder->sprite->size, 4.0);

	vrWorldAddBody(world, boulder->body);
	boulder->world = world;
	boulder->flip = vrTRUE;
	boulder->body->bodyMaterial.friction = 0.5;
	boulder->body->bodyMaterial.angularDamping = 0.4;
	boulder->sprite->center = 1;
	boulder->mp = player;

	boulder->body->collisionData.categoryMask = ENEMY;
	boulder->body->collisionData.maskBit = PLAYER | WALL;
	boulder->ft = 0.2;
	return boulder;
}

void boulder_update(enemy_container * boulderc, vrFloat dt)
{
	boulder * boulder = boulderc->enemy;
	vrCircleShape* shape = ((vrShape*)boulder->body->shape->data[0])->shape;
	boulder->sprite->pos = grV2(shape->center.x, shape->center.y);

	vrBOOL ts = vrFALSE;
	for (int i = 0; i < boulder->body->manifolds->sizeof_active; i++)
	{
		vrManifold* m = boulder->body->manifolds->data[i];
		if (!(m->A->collisionData.categoryMask == WALL || m->B->collisionData.categoryMask == WALL))
			continue;
		if (m->A == boulder->body)
		{
			if ((int)(m->normal.x - 1.0) == 0)
			{
				ts = vrTRUE;
			}
			else if ((int)(m->normal.x + 1.0) == 0)
			{
				ts = vrTRUE;
			}
		}
		else
		{
			if ((int)(m->normal.x + 1.0) == 0)
			{
				ts = vrTRUE;
			}
			else if ((int)(m->normal.x - 1.0) == 0)
			{
				ts = vrTRUE;
			}

		}
	}
	boulder->ft -= dt;
	if (ts && boulder->ft <= 0)
	{
		boulder->ft = 0.2;
		boulder->flip = !boulder->flip;
	}
	if (boulder->mp->has_crown != 0)
	{
		if (boulder->flip)
		{
			if (boulder->body->velocity.x < 350)
			{
				boulder->body->velocity.x += 10;
			}
			else boulder->body->velocity.x = 350;
		}
		else
		{
			if (boulder->body->velocity.x > -350)
			{
				boulder->body->velocity.x -= 10;
			}
			else boulder->body->velocity.x = -350;
		}
	}
}

void boulder_render(enemy_container * boulderc, grRenderer * renderer)
{
	boulder * boulder = boulderc->enemy;

	boulder->sprite->rotation = boulder->body->orientation;
	grSpriteRender(boulder->sprite, renderer);
}

void boulder_destroy(enemy_container * boulderc)
{
	boulder * boulder = boulderc->enemy;

	vrWorldRemoveBody(boulder->world, boulder->body);
	grSpriteDestroy(boulder->sprite);
	free(boulder);
}
