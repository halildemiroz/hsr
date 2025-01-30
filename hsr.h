#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define BLACK (Color){0,0,0}
#define WHITE (Color){255,255,255}
#define RED (Color){255,0,0}
#define GREEN (Color){0,255,0}
#define BLUE (Color){0,0,255}
#define LILA (Color){204,153,255}

#define RESET "\033[38m"

typedef struct{
	uint32_t r,g,b;
}Color;

typedef struct{
	Color color;
}Pixel;

typedef struct{
	Pixel* pixels;
	uint32_t width;
	uint32_t height;
}Screen;

typedef struct{
	float x,y;
}Vec2;

uint32_t rgbtohex(Color color);

Color interpolate(Color c1, Color c2, float t);

void clearscreen(Screen* screen, Color color);

void fillpixel(Screen* screen, uint32_t x, uint32_t y, Color color);

void renderscreen(Screen* screen);

void drawrectangle(Screen* screen, Vec2 startpos, Vec2 endpos, Color color);

void drawtriangle(Screen* screen, Vec2 a, Vec2 b, Vec2 c, Color color);

void drawcircle(Screen* screen, Vec2 startpos,uint32_t r, Color color);
