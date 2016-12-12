#include "..\include\enemy_manager.h"

enemy_manager * enemy_manager_alloc()
{
	return malloc(sizeof(enemy_manager));
}

enemy_manager * enemy_manager_init(enemy_manager * enemy_manager, vrWorld * world, player * p)
{
	enemy_manager->enemies = vrArrayInit(vrArrayAlloc(), sizeof(void*));
	enemy_manager->world = world;
	enemy_manager->pl = p;

	return enemy_manager;
}

void enemy_manager_add(enemy_manager * manager, vrVec2 pos, int id)
{
	enemy_container * ec = malloc(sizeof(enemy_container));
	if (id == 0)
	{
		boulder * e = boulder_init(boulder_alloc(), pos, manager->world, manager->pl);
		ec->update = &boulder_update;
		ec->render = &boulder_render;
		ec->destroy = &boulder_destroy;

		ec->enemy = e;
		ec->id = 0;

	}
	else if (id == 1)
	{
		enemy * e = enemy_init(enemy_alloc(), pos, manager->world, manager->pl);
		ec->update = &enemy_update;
		ec->render = &enemy_render;
		ec->destroy = &enemy_destroy;

		ec->enemy = e;
		ec->id = 1;

	}
	else if (id == 2)
	{
		shooter * e = shooter_init(shooter_alloc(), manager->world, pos, manager->pl, 1);
		ec->update = &shooter_update;
		ec->render = &shooter_render;
		ec->destroy = &shooter_destroy;

		ec->enemy = e;
		ec->id = 1;
	}
	else
	{
		shooter * e = shooter_init(shooter_alloc(), manager->world, pos, manager->pl, 0);
		ec->update = &shooter_update;
		ec->render = &shooter_render;
		ec->destroy = &shooter_destroy;

		ec->enemy = e;
		ec->id = 1;
	}


	vrArrayPush(manager->enemies, ec);
}

void enemy_manager_update(enemy_manager * enemy_manager, vrFloat dt)
{
	for (int i = 0; i < enemy_manager->enemies->sizeof_active; i++)
	{
		enemy_container * e = enemy_manager->enemies->data[i];
		e->update(e, dt);
	}
}

void enemy_manager_render(enemy_manager * enemy_manager, grRenderer * renderer)
{
	for (int i = 0; i < enemy_manager->enemies->sizeof_active; i++)
	{
		enemy_container * e = enemy_manager->enemies->data[i];
		e->render(e, renderer);
	}
}

void enemy_manager_destroy(enemy_manager * enemy_manager)
{
	for (int i = 0; i < enemy_manager->enemies->sizeof_active; i++)
	{
		enemy_container * e = enemy_manager->enemies->data[i];
		e->destroy(e);
		free(e);
		i = vrArrayErase(enemy_manager->enemies, i);
	}
	vrArrayDestroy(enemy_manager->enemies);
	free(enemy_manager);
}
