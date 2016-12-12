#ifndef ENEMY_CONTAINER_H
#define ENEMY_CONTAINER_H

#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>

typedef struct enemy_container enemy_container;

typedef void(*enemy_update_func)(enemy_container* container, vrFloat dt);
typedef void(*enemy_render_func)(enemy_container* container, grRenderer * renderer);
typedef void(*enemy_destroy_func)(enemy_container* container);


struct enemy_container
{
	void * enemy;
	int id;
	enemy_update_func update;
	enemy_render_func render;
	enemy_destroy_func destroy;
};

#endif