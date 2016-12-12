#include "../include/treasure.h"


treasure * treasure_alloc()
{
	return malloc(sizeof(treasure));
}

treasure * treasure_init(treasure * treasure, vrVec2 pos, vrWorld * world)
{
	treasure->body = vrBodyInit(vrBodyAlloc());

	vrShape* shape = vrShapeInit(vrShapeAlloc());
	shape = vrShapePolyInit(shape);
	shape->shape = vrPolyBoxInit(shape->shape, pos.x + 2*4, pos.y + 8*4, 12 * 4, 8 * 4);
	vrArrayPush(treasure->body->shape, shape);

	treasure->body->bodyMaterial.invMass = 1.0 / 1.0;
	treasure->body->bodyMaterial.invMomentInertia = 0;

	treasure->sprite = grSpriteInit(grSpriteAlloc(), "crown.png", 0);
	grSpriteSetUpAnimation(treasure->sprite, 1, 1, 0, 0);
	treasure->sprite->speed = 500;
	treasure->sprite->size = grVec2Scale(treasure->sprite->size, 4.0);

	vrWorldAddBody(world, treasure->body);
	treasure->world = world;

	treasure->found = 0;

	treasure->body->collisionData.categoryMask = TREASURE;
	treasure->body->collisionData.maskBit = PLAYER | WALL;
	treasure->body->bodyMaterial.invMass = 0;
	treasure->initial_pos = pos;
	
	return treasure;
}

void treasure_update(treasure * treasure, vrFloat dt)
{
	vrPolygonShape* shape = ((vrShape*)treasure->body->shape->data[0])->shape;
	vrVec2 topleft = shape->vertices[0];
	treasure->sprite->pos = grV2(topleft.x - 2*4, topleft.y - 8*4);
	grSpriteUpdate(treasure->sprite, dt, 0, 0);
	if (treasure->found)
	{
		treasure->body->collisionData.categoryMask = 0;
		treasure->body->collisionData.maskBit = 0;
		treasure->body->bodyMaterial.invMass = 0;
		treasure->body->velocity = vrVect(0, 0);
	}
	else
	{
		treasure->body->collisionData.categoryMask = TREASURE;
		treasure->body->collisionData.maskBit = PLAYER | WALL;
		treasure->body->bodyMaterial.invMass = 1;

	}
}

void treasure_render(treasure * treasure, grRenderer * renderer)
{
	if (!treasure->found) grSpriteRender(treasure->sprite, renderer);
}

void treasure_destroy(treasure * treasure)
{
	vrWorldRemoveBody(treasure->world, treasure->body);
	grSpriteDestroy(treasure->sprite);
	free(treasure);
}
