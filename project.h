#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define BLACK (Color){0,0,0}
#define WHITE (Color){255,255,255}
#define RED (Color){255,0,0}
#define GREEN (Color){0,255,0}
#define BLUE (Color){0,0,255}

#define RESET "\033[38m"

typedef struct{
	uint32_t r,g,b;
}Color;

typedef struct{
	Color color;
}Pixel;

typedef struct{
	float x,y;
}Vec2;

void clearscreen(Pixel* screen, uint32_t width, uint32_t height, Color color);

void fillpixel(Pixel* screen, uint32_t width, uint32_t height, uint32_t x, uint32_t y, Color color);

void renderscreen(Pixel* screen, uint32_t width, uint32_t height);

void drawrectangle(Pixel* screen, uint32_t width, uint32_t height, Vec2 pos, Vec2 size, Color color);

void drawtriangle(Pixel* screen, uint32_t width, uint32_t height, Vec2 a, Vec2 b, Vec2 c, Color color);
