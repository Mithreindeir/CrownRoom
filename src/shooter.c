#include "..\include\shooter.h"

shooter * shooter_alloc()
{
	return malloc(sizeof(shooter));
}

shooter * shooter_init(shooter * shooter, vrWorld * world, vrVec2 pos, player * pl, int facing)
{
	shooter->bullets = vrArrayInit(vrArrayAlloc(), sizeof(void*));
	shooter->world = world;
	shooter->pos = pos;

	shooter->sprite = grSpriteInit(grSpriteAlloc(), "hole.png", 0);
	grSpriteSetUpAnimation(shooter->sprite, 2, 1, 0, 0);
	shooter->sprite->speed = 500;
	shooter->sprite->size = grVec2Scale(shooter->sprite->size, 4.0);
	shooter->sprite->pos = grV2(pos.x, pos.y);
	shooter->pl = pl;
	shooter->dart_timer = 1.2;
	shooter->facing = facing;

	return shooter;
}

void shooter_add(shooter * shooter, vrVec2 vel)
{
	bullet * b = bullet_init(bullet_alloc(), shooter->pos, shooter->world);
	b->body->velocity = vel;
	vrArrayPush(shooter->bullets, b);
}

void shooter_update(enemy_container * shooterc, vrFloat dt)
{
	shooter * shooter = shooterc->enemy;

	shooter->dart_timer -= dt;
	if (shooter->dart_timer <= 0)
	{
		shooter->dart_timer = 1.2;
		if (shooter->facing) shooter_add(shooter, vrVect(600, 0));
		else shooter_add(shooter, vrVect(-600, 0));
	}

	for (int i = 0; i < shooter->bullets->sizeof_active; i++)
	{
		bullet * b = shooter->bullets->data[i];
		bullet_update(b, dt);
		if (b->hit)
		{
			bullet_destroy(b);
			i = vrArrayErase(shooter->bullets, i);
		}
	}
}

void shooter_render(enemy_container * shooterc, grRenderer * renderer)
{
	shooter * shooter = shooterc->enemy;
	if (shooter->facing)
		shooter->sprite->currentFrame = 0;
	else 
		shooter->sprite->currentFrame = 1;

	grSpriteRender(shooter->sprite, renderer);
	for (int i = 0; i < shooter->bullets->sizeof_active; i++)
	{
		bullet * b = shooter->bullets->data[i];
		bullet_render(b, renderer);
	}
}

void shooter_destroy(enemy_container * shooterc)
{
	shooter * shooter = shooterc->enemy;
	for (int i = 0; i < shooter->bullets->sizeof_active; i++)
	{
		bullet * b = shooter->bullets->data[i];
		bullet_destroy(b);
		i = vrArrayErase(shooter->bullets, i);
	}
	vrArrayDestroy(shooter->bullets);
	free(shooter);
}
