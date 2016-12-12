#include "../include/map.h"
#include <stdlib.h>
#include <string.h>

map * map_alloc()
{
	return malloc(sizeof(map));
}

map * map_init(map * map, const char * file)
{
	map->file = file;
	map->pos = grV2(0, 0);
	map->polygons = NULL;
	map->num_polygons = 0;
	map->tileset = NULL;
	map->height = 0;
	map->width = 0;
	FILE * f;

	fopen_s(&f, file, "r");

	//First line is tilesheet file
	//Second is tilesheeet collums and tilesheet rows with space in between
	//Third is width of map and height of map with space inbetween
	//Fourth is "START_TILES"
	//Fourth to nth is the map tile ids, with spaces in between,
	//N + 1th is "END_TILES"
	//N + 1th is "START_COLLISION"
	//This is "BEGIN_POLYGON" To "BEGIN_POLYGON"
	//WITH coordinates of points in between
	//N2 + 1th is "END_COLLISION"
	char buff[255];
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	map->player_start.x = atoi(buff);
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	map->player_start.y = atoi(buff);

	if (fscanf_s(f, "%s", buff) == -1)
	{
		return map;
	}

	map->tileset = grSpriteInit(grSpriteAlloc(), buff, 0);
	//printf("%s t\n", buff);
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	int w = atoi(buff);
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	int h = atoi(buff);
	grSpriteSetUpAnimation(map->tileset, w, h, 0, 0);
	map->tileset->size = grVec2Scale(map->tileset->size, 4.0);
	map->tileset->currentFrame = 0;
	map->tileset->speed = 500;
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	map->width = atoi(buff);
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	map->height = atoi(buff);
	if (fscanf_s(f, "%s", buff) == -1)
		return map;

	if (strcmp(buff, "START_TILES") == 0)
	{
		int * tileids = malloc(sizeof(int) * map->width  * map->height);
		int num_tiles = 0;

		if (fscanf_s(f, "%s", buff) == -1)
			return map;
		while (strcmp(buff, "END_TILES") != 0)
		{
			if (num_tiles + 1 > map->width  * map->height)
			{

				printf("ERROR READING MAP!\n");
				abort();
			}
			tileids[num_tiles] = atoi(buff);
			if (tileids[num_tiles] != 0) tileids[num_tiles] = 1;


			num_tiles++;
			if (fscanf_s(f, "%s", buff) == -1)
				return map;
		}
		map->tile_ids = tileids;
	}

	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	if (strcmp(buff, "START_COLLISION") == 0)
	{
		polygon* p = NULL;
		int num_p = 0;

		if (fscanf_s(f, "%s", buff) == -1)
			return map;
		while (strcmp(buff, "END_COLLISION") != 0)
		{
			if (num_p == 0)
			{
				p = malloc(sizeof(polygon));
				num_p++;
			}
			else
			{
				num_p++;
				p = realloc(p, num_p*sizeof(polygon));
			}

			if (strcmp(buff, "START_POLYGON") == 0)
			{

				vrVec2 *v = NULL;
				int num_v = 0;

				if (fscanf_s(f, "%s", buff) == -1)
					return map;
				while (strcmp(buff, "END_POLYGON") != 0)
				{
					if (num_v == 0)
					{
						v = malloc(sizeof(vrVec2));
						num_v++;
					}
					else
					{
						num_v++;
						int * vb = realloc(v, num_v*sizeof(vrVec2));
						if (vb)
							v = vb;
					}

					v[num_v - 1].x = atof(buff);
					if (fscanf_s(f, "%s", buff) == -1)
						return map;
					v[num_v - 1].y = atof(buff);

					if (fscanf_s(f, "%s", buff) == -1)
						return map;
				}
				p[num_p - 1].vertices = v;
				p[num_p - 1].num_vertices = num_v;
			}

			if (fscanf_s(f, "%s", buff) == -1)
				return map;
		}
		map->polygons = p;
		map->num_polygons = num_p;
	}
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	if (strcmp(buff, "START_ENEMY") == 0)
	{

		enemy_data *v = NULL;
		int num_v = 0;

		if (fscanf_s(f, "%s", buff) == -1)
			return map;
		while (strcmp(buff, "END_ENEMY") != 0)
		{
			if (num_v == 0)
			{
				v = malloc(sizeof(enemy_data));
				num_v++;
			}
			else
			{
				num_v++;
				int * vb = realloc(v, num_v*sizeof(enemy_data));
				if (vb)
					v = vb;
			}

			v[num_v - 1].pos.x = atof(buff);
			if (fscanf_s(f, "%s", buff) == -1)
				return map;
			v[num_v - 1].pos.y = atof(buff);

			if (fscanf_s(f, "%s", buff) == -1)
				return map;

			v[num_v - 1].id = atoi(buff);

			if (fscanf_s(f, "%s", buff) == -1)
				return map;
		}
		map->enemies = v;
		map->num_enemies = num_v;
	}
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	if (strcmp(buff, "START_GUN") == 0)
	{

		enemy_data *v = NULL;
		int num_v = 0;

		if (fscanf_s(f, "%s", buff) == -1)
			return map;
		while (strcmp(buff, "END_GUN") != 0)
		{
			if (num_v == 0)
			{
				v = malloc(sizeof(gun_data));
				num_v++;
			}
			else
			{
				num_v++;
				int * vb = realloc(v, num_v*sizeof(gun_data));
				if (vb)
					v = vb;
			}

			v[num_v - 1].pos.x = atof(buff);
			if (fscanf_s(f, "%s", buff) == -1)
				return map;
			v[num_v - 1].pos.y = atof(buff);

			if (fscanf_s(f, "%s", buff) == -1)
				return map;

			v[num_v - 1].id = atoi(buff);

			if (fscanf_s(f, "%s", buff) == -1)
				return map;
		}
		map->guns = v;
		map->num_guns = num_v;
	}
	if (fscanf_s(f, "%s", buff) == -1)
		return map;
	if (strcmp(buff, "START_DESTRUCTABLE") == 0)
	{

		vrVec2 *v = NULL;
		int num_v = 0;

		if (fscanf_s(f, "%s", buff) == -1)
			return map;
		while (strcmp(buff, "END_DESTRUCTABLE") != 0)
		{
			if (num_v == 0)
			{
				v = malloc(sizeof(vrVec2));
				num_v++;
			}
			else
			{
				num_v++;
				int * vb = realloc(v, num_v*sizeof(vrVec2));
				if (vb)
					v = vb;
			}

			v[num_v - 1].x = atof(buff);
			if (fscanf_s(f, "%s", buff) == -1)
				return map;
			v[num_v - 1].y = atof(buff);

			if (fscanf_s(f, "%s", buff) == -1)
				return map;
		}
		map->destructables = v;
		map->num_destructables = num_v;
	}
	fclose(f);
	return map;
}

void map_draw(map * map, grRenderer * renderer)
{
	grShaderUse(renderer->shader);

	glBindVertexArray(renderer->vao);
	if (renderer->bound_tex == -1 || renderer->bound_tex != map->tileset->texture->id)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, map->tileset->texture->id);
		renderer->bound_tex = map->tileset->texture->id;
	}

	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			if (map->tile_ids[j + i * map->width] > 0)
			{
				int up, down, left, right;
				if (i > 0)
					up = !!map->tile_ids[j + (i - 1) * map->width];
				else
					up = 1;
				if (j > 0)
					left = !!map->tile_ids[j - 1 + (i)* map->width];
				else
					left = 1;
				if ((j + 1) < map->width)
					right = !!map->tile_ids[j + 1 + (i)* map->width];
				else
					right = 1;
				if ((i + 1) < map->height)
					down = !!map->tile_ids[j + (i + 1)* map->width];
				else
					down = 1;
				map->tile_ids[j + i * map->width] = get_tile_index(up, down, left, right);
				if (map->tile_ids[j + i * map->width] > 0) map->tile_ids[j + i * map->width] += 1;
			}
		}
	}
	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			if (map->tile_ids[j + i * map->width] > 0)
			{
				//continue;
				if (map->tile_ids[j + i * map->width] == 15)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
				else if (map->tile_ids[j + i * map->width] == 7)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
				else if (map->tile_ids[j + i * map->width] == 13)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
				else if (map->tile_ids[j + i * map->width] == 3)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
				else if (map->tile_ids[j + i * map->width] == 5)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
				else if (map->tile_ids[j + i * map->width] == 9)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
				else if (map->tile_ids[j + i * map->width] == 11)
				{
					if (i > 0)
					{
						int p = i - 1;
						if (map->tile_ids[j + p * map->width] == 0) {
							map->tile_ids[j + p * map->width] = 17;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			grVec2 pos = grV2(j * 16 * 4, i * 16 * 4);
			pos = grVec2Add(map->pos, pos);
			if (pos.x + 16 * 4 < renderer->camera->pos.x)
				continue;
			if (pos.x  > renderer->camera->pos.x + renderer->camera->zoom.view.x)
				continue;
			if (pos.y + 16 * 4 < renderer->camera->pos.y)
				continue;
			if (pos.y  > renderer->camera->pos.y + renderer->camera->zoom.view.y)
				continue;
			int tile = map->tile_ids[j + i * map->width];
			if (tile > 0)
			{
				map->tileset->pos = pos;
				grSprite * sprite = map->tileset;

				grMat4 model = grMat4Identity();
				grMat4 view = grCameraGetViewMatrix(renderer->camera);

				model = grMat4Translate(model, grV4(sprite->pos.x, sprite->pos.y, 0.0, 1.0));
				//model = grMat4Translate(model, grV4(0.5f * sprite->size.x, 0.5f * sprite->size.y, 0.0, 1.0));
				//model = grMat4Rotate(model, grV4(0, 0, 1.0, 0), sprite->rotation);
				//model = grMat4Translate(model, grV4(-0.5f * sprite->size.x, -0.5f * sprite->size.y, 0.0, 1.0));
				model = grMat4Scale(model, grV4(sprite->size.x, sprite->size.y, 0.0, 1.0));

				grShaderSetMat4(renderer->shader, "model", model);
				grShaderSetVec4(renderer->shader, "spriteColor", sprite->color);
				grShaderSetMat4(renderer->shader, "view", view);

				if (map->tileset->quads[tile - 1] != NULL)
				{
					GLfloat v[24];
					memcpy(v, map->tileset->quads[tile - 1]->vertices, sizeof(v));
					glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
					glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_DYNAMIC_DRAW);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
				}
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
	}
	glBindVertexArray(0);

	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			int tile = map->tile_ids[j + i * map->width];
			if (tile > 0 && tile < 17)
				map->tile_ids[j + i * map->width] = 1;
			else
				map->tile_ids[j + i * map->width] = 0;

		}
	}
}

void map_destroy(map * map)
{
	free(map->tile_ids);
	free(map->destructables);
	free(map->enemies);
	grSpriteDestroy(map->tileset);
	for (int i = 0; i < map->num_polygons; i++)
	{
		free(map->polygons[i].vertices);
	}
	free(map->polygons);
	free(map);
	map = NULL;
}