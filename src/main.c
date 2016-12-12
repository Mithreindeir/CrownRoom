#include <stdio.h>
#include <conio.h>
#include <graphicsraptor\graphicsraptor.h>
#include <graphicsraptor\grSprite.h>
#include <graphicsraptor\grWindow.h>
#include <velocityraptor\VelocityRaptor.h>
#include <velocityraptor\vrRaycast.h>
#include <velocityraptor\vrRigidBody.h>
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/treasure.h"
#include "../include/enemy_manager.h"
#include "../include/map.h"
#include "../include/shooter.h"
#include "../include/text.h"

#define DEBUG 0

GLchar* default_vert = "#version 330 core\n"
"layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n"

"out vec2 TexCoords; \n"

"uniform mat4 model; \n"
"uniform mat4 projection; \n"
"uniform mat4 view; \n"

"void main()\n"
"{\n"
"	TexCoords = vertex.zw; \n"
"	gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0); \n"
"}\n\0";

GLchar* default_frag = "#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"

"uniform sampler2D image;\n"
"uniform vec4 spriteColor;\n"

"void main()\n"
"{\n"
"	color = spriteColor * texture(image, TexCoords);\n"
"}\n\0";

text_renderer * textr;
grWindow * wind;
vrWorld * world;
player * main_player;
grRenderer * renderer;
enemy_manager * enemy_m;
treasure * main_treasure;
map * main_map;
music * song;
grSprite * win;
grSprite * start;


int w = 768, h = 768;

void keyp(int key);
void keyr(int key);
void main_loop(void * ud);

#define NUM_LEVELS 7
const static char * maps[NUM_LEVELS] = {
	"map.txt", "map2.txt", "map3.txt", "map4.txt", "map5.txt", "map6.txt", "map7.txt"
};


void init(int level);
void destroy();
void start_state(void * d);
void win_state(void * d);
int started = 0;
int current_level = 0;

int main(int argc, char* argv[])
{
	//
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		printf("AUDIO ERROR\n");
	}
	Mix_AllocateChannels(320);
	grInit();
	wind = grWindowInit(grWindowAlloc(), w, h);
	glfwSetWindowTitle(wind->window, "Crown Room");

	//wind->background_color = grV4(69.0/255.0, 40.0/255.0, 60.0/255.0, 1);
	keypressed = &keyp;
	keyreleased = &keyr;

	song = music_init(music_alloc(), "song.wav");

	music_play(song);
	win = grSpriteInit(grSpriteAlloc(), "crown.png", 0);
	grSpriteSetUpAnimation(win, 1, 1, 0, 0);
	win->pos = grV2(0, 0);
	win->size = grVec2Scale(win->size, 48.0);

	start = grSpriteInit(grSpriteAlloc(), "howto.png", 0);
	grSpriteSetUpAnimation(start, 206, 1, 0, 0);
	start->pos = grV2(0, 0);
	start->size = grVec2Scale(start->size, 16.0);
	start->speed = 200;

	renderer = grRendererInit(grRendererAlloc(), grV2(w, h));
	grMat4 proj = grCameraGetProjectionMatrix(renderer->camera);
	grShader* default_shader = grShaderInit(grShaderAlloc());
	grShaderCompile(default_shader, default_vert, default_frag);
	renderer->shader = default_shader;
	renderer->camera->speed = grV2(2.6, 2.6);

	textr = text_renderer_init(text_renderer_alloc());

	init(current_level);
	wind->userFunc = &main_loop;

	//
	grMainLoop(wind);

	//
	destroy();
	grDestroy(wind);
	music_destroy(song);
	grSpriteDestroy(start);
	grSpriteDestroy(win);
	grRendererDestroy(renderer);
	text_renderer_destroy(textr);

	return 1;
}
grFloat ct2 = 0, lt2 = 0;
void start_state(void * d)
{
	grMat4 proj = grCameraGetProjectionMatrix(renderer->camera);
	grShaderUse(renderer->shader);
	grShaderSetInteger(renderer->shader, "image", 0);
	grShaderSetMat4(renderer->shader, "projection", proj);

	ct2 = glfwGetTime();
	vrFloat dt = ct2 - lt2;
	lt2 = ct2;
	grSpriteUpdate(start, dt, 0, 206);
	grSpriteRender(start, renderer);
	//render_text(textr, renderer, "CROWN ROOM!", grV2(4 * 16 * 4, 2 * 16 * 4));
}

void win_state(void * d)
{
	grMat4 proj = grCameraGetProjectionMatrix(renderer->camera);
	grShaderUse(renderer->shader);
	grShaderSetInteger(renderer->shader, "image", 0);
	grShaderSetMat4(renderer->shader, "projection", proj);
	grSpriteRender(win, renderer);
	render_text(textr, renderer, "YOU WIN!", grV2(4 * 16 * 4, 2 * 16 * 4));
}

void init(int level)
{
	world = vrWorldInit(vrWorldAlloc());
	world->gravity = vrVect(0, GRAVITY);
	world->timeStep = 1.0 / 120.0;

	main_player = player_init(player_alloc(), vrVect(8 * 4, 50), world, NULL);
	//main_treasure = treasure_init(treasure_alloc(), vrVect(384, 768 - (16*4)), world);

	enemy_m = enemy_manager_init(enemy_manager_alloc(), world, main_player);
	//enemy_manager_add(enemy_m, vrVect(w / 2.0, h / 2.0));
	//enemy_manager_add(enemy_m, vrVect(100, h / 2.0));
	//enemy_manager_add(enemy_m, vrVect(110, h / 2.0));
	//enemy_manager_add(enemy_m, vrVect(120, h / 2.0));
	//enemy_manager_add(enemy_m, vrVect(130, h / 2.0));
	//enemy_manager_add(enemy_m, vrVect(140, h / 2.0));
	//enemy_manager_add(enemy_m, vrVect(150, h / 2.0));
	vrRigidBody * body = vrBodyInit(vrBodyAlloc());

	vrShape* shape = vrShapeInit(vrShapeAlloc());
	shape = vrShapePolyInit(shape);
	shape->shape = vrPolyBoxInit(shape->shape, -100, 0, 100, 800);
	vrArrayPush(body->shape, shape);

	body->bodyMaterial.invMass = 0;
	body->bodyMaterial.invMomentInertia = 0;
	vrWorldAddBody(world, body);

	srand(time(NULL));

	main_map = map_init(map_alloc(), maps[level]);
	main_treasure = treasure_init(treasure_alloc(), vrVect(main_map->player_start.x*4*16, main_map->player_start.y*4*16), world);
	main_player->tr = main_treasure;

	for (int i = 0; i < main_map->num_enemies; i++)
	{
		enemy_manager_add(enemy_m, vrScale(main_map->enemies[i].pos, 4*16), main_map->enemies[i].id);
	}
	create_walls_from_grid(main_map->tile_ids, main_map->width, main_map->height, world);
}
void destroy()
{
	map_destroy(main_map);
	treasure_destroy(main_treasure);
	player_destroy(main_player);
	enemy_manager_destroy(enemy_m);
	vrWorldDestroy(world);
}

vrBOOL exclude2(vrRigidBody* body)
{
	return vrTRUE;
}

vrFloat lt = 0, ct = 0;
vrFloat timer = 15;

int taken_crown = 0;

void main_loop(void * ud)
{
	int w, h;
	glfwGetWindowSize(wind->window, &w, &h);
	grFloat optW = (float)h;
	grFloat optH = (float)w;
	if (optW > w)
	{
		int x = 0;
		int width = w;
		float e = (float)h - optH;
		int h = round(0.5 * e);
		int uh = round(optH);
		int y = h;
		int height = uh;
		glViewport(x, y, width, height);
	}
	else
	{
		int y = 0;
		int height = h;
		float e = (float)w - optW;
		int w = round(0.5 * e);
		int uw = round(optW);
		int x = w;
		int width = uw;
		glViewport(x, y, width, height);
	}

	if (main_player->body->center.x < 50 && main_player->has_crown != 0 && current_level < (NUM_LEVELS-1))
	{
		destroy();
		current_level++;
		init(current_level);
		timer = 15;
	}
	if (main_player->body->center.x < 50 && main_player->has_crown != 0 && current_level == (NUM_LEVELS-1))
	{
		wind->userFunc = &win_state;
		return;
	}
	if ((int)timer <= 0)
	{
		destroy();
		init(current_level);
		timer = 15;
	}
	grMat4 proj = grCameraGetProjectionMatrix(renderer->camera);
	grShaderUse(renderer->shader);
	grShaderSetInteger(renderer->shader, "image", 0);
	grShaderSetMat4(renderer->shader, "projection", proj);
	
	//
	vrWorldStep(world);
	if (keys[GLFW_KEY_LEFT])
	{	
		if (main_player->body->angularVelocity > -20)
			main_player->body->angularVelocity -= 5;
		else
			main_player->body->angularVelocity = -20;

		if (main_player->body->velocity.x > -450)
		{
			main_player->body->velocity.x -= 10;
		}
		else main_player->body->velocity.x = -450;
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		if (main_player->body->angularVelocity < 20)
			main_player->body->angularVelocity += 5;
		else
			main_player->body->angularVelocity = 20;
		
		if (main_player->body->velocity.x < 450)
		{
			main_player->body->velocity.x += 10;
		}
		else main_player->body->velocity.x = 450;
		
	}
	ct = glfwGetTime();
	vrFloat dt = ct - lt;
	lt = ct;

	//
	player_update(main_player, dt);
	enemy_manager_update(enemy_m, dt);
	treasure_update(main_treasure, dt);

	grCameraUpdate(renderer->camera, dt);

	if (taken_crown == 0 && main_player->has_crown != 0)
	{
		grCameraShakeStart(renderer->camera, 4.5, 4.5, 65);
	}
	if (main_player->has_crown != 0)
	{
		if (main_player->body->center.x > 75) timer -= dt;

		taken_crown = 1;
	}
	else taken_crown = 0;
	//
	map_draw(main_map, renderer);
	player_render(main_player, renderer);
	enemy_manager_render(enemy_m, renderer);
	treasure_render(main_treasure, renderer);
	char buff[16];
	_itoa_s((int)timer, buff, 16, 10);

	render_text(textr, renderer, buff, grV2(16*4*2, 16*4*1));
	
	if (main_player->has_crown == 0) render_text(textr, renderer, "GET THE CROWN!", grV2(16 * 4 * 4, 16 * 4 * 1));
	else if (timer > 10) render_text(textr, renderer, "ESCAPE!", grV2(16 * 4 * 4, 16 * 4 * 1));
	else if (timer > 5) render_text(textr, renderer, "QUICK! ESCAPE!", grV2(16 * 4 * 4, 16 * 4 * 1));
	else if (timer > 0) render_text(textr, renderer, "GO! GO! GO!", grV2(16 * 4 * 4, 16 * 4 * 1));

	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 768, 768, 0 + 1.f, 1.f, -1.f);
	glTranslatef(-renderer->camera->pos.x, -renderer->camera->pos.y, 0.0);
	glColor3f(143.0/255.0, 86.0/255.0, 59.0/255.0);
	glPointSize(8);
	if (main_player->has_rope)
	{
		vrDistanceConstraint * dc = main_player->rope->jointData;

		vrVec2 a, b;
		a = main_player->body->center;
		b = vrAdd(main_player->rope->B->center, dc->rb);
		glLineWidth(8);
		glBegin(GL_LINES);
		glVertex2f(a.x, a.y);
		glVertex2f(b.x, b.y);
		glEnd();
	}
	if (DEBUG)
	{

		
		vrCircleShape* shape = ((vrShape*)main_player->body->shape->data[0])->shape;
		vrVec2 pos = vrAdd(shape->center, vrScale(vrVect(0, 1), shape->radius));
		vrRaycastInput input;
		vrRaycastOutput output;
		input.start = vrVect(pos.x - 0.01, pos.y - 8);
		input.end = vrAdd(pos, vrVect(main_player->flip ? 1500 : -1500, -1500));
		input.excludeFunc = &exclude2;
		output = vrRaycast(main_player->world, input);
		glBegin(GL_LINES);
		glVertex2f(input.start.x, input.start.y);
		glVertex2f(output.point.x, output.point.y);
		

		//glVertex2f(p->g->gun->pos.x, p->g->gun->pos.y);
		glEnd();
		glColor3f(0.5, 0.5, 0.5);
		for (int i = 0; i < world->bodies->sizeof_active; i++)
		{
			vrRigidBody* body = world->bodies->data[i];
			if (((vrShape*)body->shape->data[0])->shapeType == VR_POLYGON)
			{
				vrPolygonShape* shape = ((vrShape*)body->shape->data[0])->shape;

				glBegin(GL_LINE_LOOP);

				for (int i = 0; i < shape->num_vertices; i++)
				{
					glVertex2f(shape->vertices[i].x, shape->vertices[i].y);
				}
				glEnd();
			}
			else {
				vrCircleShape* shape = ((vrShape*)body->shape->data[0])->shape;

				glPointSize(32);
				glBegin(GL_POINTS);
				//glVertex2f(shape->center.x, shape->center.y);
				glEnd();
			}
		}
	}
}

void keyp(int key)
{
	if (!started)
	{
		wind->userFunc = &main_loop;
		started = 1;
		return;
	}
	if (key == GLFW_KEY_R && main_player->body->center.x > 90)
	{
		destroy();
		init(current_level);
		timer = 15;
	}
	if (key == GLFW_KEY_SPACE)
	{
		vrCircleShape* shape = ((vrShape*)main_player->body->shape->data[0])->shape;
		vrVec2 pos = vrAdd(shape->center, vrScale(vrVect(0, 1), shape->radius));
		vrRaycastInput input;
		vrRaycastOutput output;
		input.start = vrVect(pos.x - 0.01, pos.y - 8);
		input.end = vrAdd(pos, vrVect(main_player->flip ? 1500 : -1500, -1500));
		input.excludeFunc = &exclude2;
		output = vrRaycast(main_player->world, input);

		if (output.intersection)
		{
			sound_play(main_player->rope_sound);

			main_player->has_rope = 1;
			main_player->rope = vrDistanceConstraintInit(vrJointAlloc(), main_player->body, output.body, main_player->body->center, output.point);

			vrDistanceConstraint * dc = main_player->rope->jointData;
			dc->max = 1;
			vrArrayPush(world->joints, main_player->rope);
		}
	}
	if (key == GLFW_KEY_UP && !main_player->on_ground)
	{
		if (main_player->on_wall)
		{
			sound_play(main_player->jump);
			if (main_player->rw)
			{
				main_player->body->velocity.y = -JUMP_SPEED/1.5;
				main_player->body->velocity.x = -JUMP_SPEED/2.0;
			}
			else
			{
				main_player->body->velocity.y = -JUMP_SPEED / 1.5;
				main_player->body->velocity.x = JUMP_SPEED / 2.0;
			}
		}
	}
	else if (key == GLFW_KEY_UP)
	{
		sound_play(main_player->jump);
		main_player->body->velocity.y = -JUMP_SPEED;
	}
}
void keyr(int key)
{
	if (key == GLFW_KEY_SPACE && main_player->rope)
	{
		vrArrayErase(world->joints, 0);
		main_player->has_rope = 0;
		vrDistanceConstraintDestroy(main_player->rope);
		main_player->rope = NULL;
	}
	if (key == GLFW_KEY_UP && !main_player->on_ground)
	{

		if (main_player->body->velocity.y < -EARLY_TERMINATION)
			main_player->body->velocity.y = -EARLY_TERMINATION;
	}
}

