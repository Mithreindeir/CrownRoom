#include "../include/text.h"

text_renderer * text_renderer_alloc()
{
	return malloc(sizeof(text_renderer));
}

text_renderer * text_renderer_init(text_renderer * textr)
{
	textr->bitmap_font = grSpriteInit(grSpriteAlloc(), "font.png", 0);
	grSpriteSetUpAnimation(textr->bitmap_font, 18, 7, 0, 0);
	textr->bitmap_font->speed = 0;
	textr->bitmap_font->size = grVec2Scale(textr->bitmap_font->size, 4.0);

	return textr;
}

void render_text(text_renderer * textr, grRenderer * renderer, char * text, grVec2 pos)
{
	int size = sizeof(temp);

	for (int j = 0; j < strlen(text); j++)
	{
		char c = text[j];
		for (int i = 0; i < size; i++)
		{
			if (temp[i] == c)
			{
				textr->bitmap_font->currentFrame = i;
				textr->bitmap_font->pos = pos;
				grSpriteRender(textr->bitmap_font, renderer);
				pos.x += textr->bitmap_font->size.x;
				break;
			}
		}
	}
}

void text_renderer_destroy(text_renderer * textr)
{
	grSpriteDestroy(textr->bitmap_font);
	free(textr);
}
