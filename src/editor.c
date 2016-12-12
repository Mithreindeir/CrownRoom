#include "../include/editor.h"

int get_tile_index(int top, int bottom, int left, int right)
{
	int sum = 0;
	if (top) sum += 1;
	if (left) sum += 2;
	if (bottom) sum += 4;
	if (right) sum += 8;

	return sum;
}

void create_walls_from_grid(int * ids, int map_width, int map_height, vrWorld * world)
{

	int rootBlockX, rootBlockY;
	rootBlockX = 0;
	rootBlockY = 0;
	//If it is 3 it means floor
	//If it is 2 it is used
	//if it is 1 its open
	int* roomMap = malloc(sizeof(int) * map_width * map_height);
	memset(roomMap, 3, map_width * map_height);
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			if (ids[j + i*map_width] > 0)
			{
				roomMap[j + i*map_width] = 1;
			}
			else roomMap[j + i*map_width] = 3;
		}
	}
	int currentX, currentY;
	currentX = 0;
	currentY = 0;
	for (;;)
	{

		int end = vrFALSE;
		int width, height;
		width = 0;
		height = 0;
		//Find block

		for (int i = 0; i < map_height; i++)
		{
			for (int j = 0; j < map_width; j++)
			{
				if (roomMap[j + i * map_width] == 1)
				{
					currentX = j;
					currentY = i;
					rootBlockX = j;
					rootBlockY = i;
					end = vrTRUE;
					roomMap[j + i * map_width] = 2;
				}
				if (end) break;
			}
			if (end) break;
		}
		if (!end) break;
		//Make a rect
		vrOrientedBoundingBox wall;
		wall = vrOBBCreate(vrVect(rootBlockX, rootBlockY), vrVect(width + 1, height + 1));
		//Extend it as far out horizontally
		for (int i = rootBlockX + 1; i < map_width; i++)
		{
			if (roomMap[i + rootBlockY * map_width] == 1)
			{
				wall.size.x++;
				width++;
				roomMap[i + rootBlockY * map_width] = 2;
			}
			else
				break;
		}
		vrBOOL finish = vrFALSE;
		//Now as far out vertically
		for (int i = rootBlockY + 1; i < map_height; i++)
		{
			for (int j = rootBlockX; j < rootBlockX + width + 1; j++)
			{
				if (roomMap[j + i * map_width] != 1)
					finish = vrTRUE;
			}
			if (!finish)
			{
				for (int j = rootBlockX; j < (rootBlockX + width + 1); j++)
				{
					roomMap[j + i * map_width] = 2;
				}
				wall.size.y++;
				height++;
			}
			else break;
		}

		vrRigidBody* body = vrBodyInit(vrBodyAlloc());
		vrShape* shape = vrShapeInit(vrShapeAlloc());
		shape = vrShapePolyInit(shape);
		shape->shape = vrPolyBoxInit(shape->shape, wall.position.x * 16 * 4, wall.position.y * 16 * 4, wall.size.x * 16 * 4, wall.size.y * 16 * 4);
		vrArrayPush(body->shape, shape);

		body->bodyMaterial.invMass = 0.0 / 1.0;
		body->bodyMaterial.invMomentInertia = 0;
		body->bodyMaterial.friction = 0.0;
		body->velocity = vrVect(0, 0);
		vrWorldAddBody(world, body);
	}

	free(roomMap);

}
