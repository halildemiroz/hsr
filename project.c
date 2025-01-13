#include "project.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void drawtriangle(Screen* screen, Vec2 vert1, Vec2 vert2, Vec2 vert3, Color color){
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
				fillpixel(screen, x, y, color);
			}
		}
	}
}

void drawcircle(Screen* screen, Vec2 startpos, uint32_t r, Color color){
	// x2 + y2 = r2
	// merkeze olan uzaklığı hesapla ve yarıçaptan fazlaysa o noktayı alma
	uint32_t d;
	// Center[0] = centerX Center[1] = centerY
	uint32_t center[] = {startpos.x + r, startpos.y + r};
	for(uint32_t y = startpos.y; y <= center[1] + r; y++){
		for(uint32_t x = startpos.x; x <= center[0] + r; x++){
			d = sqrt(((center[0] - x)*(center[0] - x)) + ((center[1] - y))*(center[1] - y));
			if(d <= r)
				fillpixel(screen, x, y, color);
		}
	}
}
