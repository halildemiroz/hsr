#include "hsr.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

uint32_t rgbtohex(Color color){
    return ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
}

float findmin(float* vals, uint32_t n){
	float min = vals[0];
	for(uint32_t i = 0; i < n; i++){
		if(vals[i] < min)
			min = vals[i];
	}
	return min;
}

float findmax(float* vals, uint32_t n){
	float max = vals[0];
	for(uint32_t i = 0; i < n; i++){
		if(vals[i] > max)
			max = vals[i];
	}
	return max;
}

Vec2 vec2sub(Vec2 a, Vec2 b){
	return (Vec2){
		.x = a.x - b.x,
		.y = a.y - b.y 
	};
}

float getdeterminant(Vec2 edgestart, Vec2 edgeend, Vec2 pixel){
		Vec2 edgelen = vec2sub(edgeend, edgestart);
		Vec2 pixeldist = vec2sub(pixel, edgestart);

		return edgelen.x * pixeldist.y - edgelen.y * pixeldist.x;
}

Vec3 getBarycentricCoords(Vec2 vert1, Vec2 vert2, Vec2 vert3, Vec2 point) {
    Vec2 v0 = vec2sub(vert2, vert1);
    Vec2 v1 = vec2sub(vert3, vert1);
    Vec2 v2 = vec2sub(point, vert1);

    float d00 = v0.x * v0.x + v0.y * v0.y;
    float d01 = v0.x * v1.x + v0.y * v1.y;
    float d11 = v1.x * v1.x + v1.y * v1.y;
    float d20 = v2.x * v0.x + v2.y * v0.y;
    float d21 = v2.x * v1.x + v2.y * v1.y;

    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return (Vec3){u, v, w};
}

Color interpolateColor(Vec3 baryCoords, Color colorA, Color colorB, Color colorC) {
    return (Color){
        .r = baryCoords.x * colorA.r + baryCoords.y * colorB.r + baryCoords.z * colorC.r,
        .g = baryCoords.x * colorA.g + baryCoords.y * colorB.g + baryCoords.z * colorC.g,
        .b = baryCoords.x * colorA.b + baryCoords.y * colorB.b + baryCoords.z * colorC.b
    };
}

Screen* createscreen(uint32_t width, uint32_t height){
	Screen* screen = malloc(sizeof(*screen));
	screen->width = width;
	screen->height = height;
	screen->pixels = malloc(sizeof(Pixel) * width * height);
	return screen;
}

void clearscreen(Screen* screen, Color color){
	for(uint32_t i = 0; i < screen->width * screen->height; i++){
		screen->pixels[i].color = color;
	}
}

void fillpixel(Screen* screen, uint32_t x, uint32_t y, Color color){
	if(y>=screen->height || x>=screen->width) return;
	screen->pixels[(y * screen->width) + x].color = color;
}

void renderscreen(Screen* screen){
	for(uint32_t i = 0; i < screen->height * screen->width; i++){
		Pixel pixels = screen->pixels[i];
		if(i % screen->width == 0)
			printf("\n");
		printf("\033[38;2;%u;%u;%umoo" RESET, pixels.color.r,pixels.color.g,pixels.color.b);
	}
}

void drawrectangle(Screen* screen, Vec2 startpos, Vec2 endpos, Color color){

	float xvals[] = {startpos.x, endpos.x};
	float yvals[] = {startpos.y, endpos.y};

	float xmin = findmin(xvals, 2);
	float ymin = findmin(yvals, 2);

	float xmax = findmax(xvals, 2);
	float ymax = findmax(yvals, 2);

	for(int32_t y = ymin; y <= ymax; y++){
		for(int32_t x = xmin; x <= xmax; x++){
			fillpixel(screen, x, y, color);
		}
	}
}

// Flag is used to determine if we want to triangle to be colorful or not
void drawtriangle(Screen* screen, Vec2 vert1, Vec2 vert2, Vec2 vert3, Color color, int flag) {
  	float xvals[] = {vert1.x, vert2.x, vert3.x};
	float yvals[] = {vert1.y, vert2.y, vert3.y};

    float xmin = findmin(xvals, 3);
    float ymin = findmin(yvals, 3);

    float xmax = findmax(xvals, 3);
    float ymax = findmax(yvals, 3);

    for(int32_t y = ymin; y <= ymax; y++){
        for(int32_t x = xmin; x <= xmax; x++){
            Vec2 point = (Vec2){.x = x, .y = y};

           float d1 = getdeterminant(vert2, vert3, point); 
		   float d2 = getdeterminant(vert3, vert1, point);
		   float d3 = getdeterminant(vert1, vert2, point);

            if(d1 >= 0 && d2 >= 0 && d3 >= 0){
				if(flag){				
                	Vec3 baryCoords = getBarycentricCoords(vert1, vert2, vert3, point);
                	Color newcolor = interpolateColor(baryCoords, RED, GREEN, BLUE);
					fillpixel(screen, x, y, newcolor);
				}
				else{
					fillpixel(screen, x, y, color);
				}
            }
        }
    }
}

void drawcircle(Screen* screen, Vec2 startpos, uint32_t r, Color color){
	// x2 + y2 = r2
	uint32_t d;
	// Center[0] = centerX, Center[1] = centerY
	uint32_t center[] = {startpos.x + r, startpos.y + r};
	for(uint32_t y = startpos.y; y <= center[1] + r; y++){
		for(uint32_t x = startpos.x; x <= center[0] + r; x++){
			d = sqrt(((center[0] - x)*(center[0] - x)) + ((center[1] - y))*(center[1] - y));
			if(d <= r)
				fillpixel(screen, x, y, color);
		}
	}
}
