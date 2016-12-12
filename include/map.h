#ifndef HEADER_MAP
#define HEADER_MAP

#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRigidBody.h>
#include <graphicsraptor\grSprite.h>
#include "editor.h"
#include <stdio.h>

typedef struct polygon
{
	int num_vertices;
	vrVec2* vertices;
} polygon;

typedef struct enemy_data
{
	vrVec2 pos;
	int id;
} enemy_data;

typedef struct gun_data
{
	vrVec2 pos;
	int id;
} gun_data;

typedef struct map
{
	grVec2 player_start;
	grVec2 pos;
	char* file;
	grSprite* tileset;
	int width;
	int height;
	int* tile_ids;
	polygon* polygons;
	int num_polygons;
	enemy_data * enemies;
	int num_enemies;
	gun_data * guns;
	int num_guns;
	vrVec2 * destructables;
	int num_destructables;
} map;

map * map_alloc();
map * map_init(map * map, const char * file);
void map_draw(map * map, grRenderer * renderer);
void map_destroy(map* map);
/*
format
polygon:
{
32 32,
64 32,
64 64,
}
*/
typedef struct node
{
	char* name;
	char** data;
	int num_data;
} node;

#endif