#include "../include/bullet.h"


bullet * bullet_alloc()
{
	return malloc(sizeof(bullet));
}

bullet * bullet_init(bullet * bullet, vrVec2 pos, vrWorld * world)
{
	bullet->body = vrBodyInit(vrBodyAlloc());

	vrShape* shape = vrShapeInit(vrShapeAlloc());
	shape = vrShapePolyInit(shape);
	shape->shape = vrPolyBoxInit(shape->shape, pos.x + 4*4, pos.y + 6 * 4, 9 * 4, 3 * 4);
	vrArrayPush(bullet->body->shape, shape);

	bullet->body->bodyMaterial.invMass = 1.0 / 1.0;
	bullet->body->bodyMaterial.invMomentInertia = 0;

	bullet->sprite = grSpriteInit(grSpriteAlloc(), "dart.png", 0);
	grSpriteSetUpAnimation(bullet->sprite, 2, 1, 0, 0);
	bullet->sprite->speed = 500;
	bullet->sprite->size = grVec2Scale(bullet->sprite->size, 4.0);

	vrWorldAddBody(world, bullet->body);
	bullet->world = world;

	bullet->hit = vrFALSE;

	bullet->body->collisionData.categoryMask = ENEMY_BULLET;
	bullet->body->collisionData.maskBit = PLAYER | WALL;
	bullet->body->bodyMaterial.invMass = 1.0 / 10.0;
	bullet->initial_pos = pos;
	bullet->body->gravity = 0;
	vrPolygonShape* pshape = ((vrShape*)bullet->body->shape->data[0])->shape;
	vrVec2 topleft = pshape->vertices[0];
	bullet->sprite->pos = grV2(topleft.x - 4 * 4, topleft.y - 6 * 4);

	return bullet;
}

void bullet_update(bullet * bullet, vrFloat dt)
{
	vrPolygonShape* shape = ((vrShape*)bullet->body->shape->data[0])->shape;
	vrVec2 topleft = shape->vertices[0];
	bullet->sprite->pos = grV2(topleft.x - 4 * 4, topleft.y - 6 * 4);
	grSpriteUpdate(bullet->sprite, dt, 0, 0);
	if (bullet->hit)
	{
		bullet->body->collisionData.categoryMask = 0;
		bullet->body->collisionData.maskBit = 0;
		bullet->body->bodyMaterial.invMass = 0;
		bullet->body->velocity = vrVect(0, 0);
	}
	else
	{
		bullet->body->collisionData.categoryMask = ENEMY_BULLET;
		bullet->body->collisionData.maskBit = PLAYER | WALL;
		bullet->body->bodyMaterial.invMass = 1.0 / 1.0;
	}

	for (int i = 0; i < bullet->body->manifolds->sizeof_active; i++)
	{
		vrManifold* m = bullet->body->manifolds->data[i];
		if (m->A->collisionData.categoryMask == WALL || m->B->collisionData.categoryMask == WALL || m->A->collisionData.categoryMask == PLAYER || m->B->collisionData.categoryMask == PLAYER)
		{
			bullet->hit = vrTRUE;
		}
	}
	if (bullet->body->velocity.x > 0)
		bullet->sprite->currentFrame = 0;
	else 
		bullet->sprite->currentFrame = 1;
}

void bullet_render(bullet * bullet, grRenderer * renderer)
{
	if (!bullet->hit) grSpriteRender(bullet->sprite, renderer);
}

void bullet_destroy(bullet * bullet)
{
	vrWorldRemoveBody(bullet->world, bullet->body);
	grSpriteDestroy(bullet->sprite);
	free(bullet);
}
