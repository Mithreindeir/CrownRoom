#ifndef TEXT_H
#define TEXT_H

#include <graphicsraptor\grSprite.h>

const static char temp[] = {
	' ', '!', '"', '#', '$', '%', '&', ' ', '(', ')', '*', '+',
	',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', '[', ' ', ']', '^', '_', '`',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
	'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
};


typedef struct text_renderer
{
	grSprite * bitmap_font;

} text_renderer;

text_renderer * text_renderer_alloc();
text_renderer * text_renderer_init(text_renderer* textr);
void render_text(text_renderer * textr, grRenderer * renderer, char * text, grVec2 pos);
void text_renderer_destroy(text_renderer * textr);

#endif