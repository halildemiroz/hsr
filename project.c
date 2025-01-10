#include "project.h"

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

void clearscreen(Pixel* screen, uint32_t width, uint32_t height, Color color){
	for(uint32_t i = 0; i < width * height; i++){
		screen[i].color = color;
	}
}

void fillpixel(Pixel* screen, uint32_t width, uint32_t height, uint32_t x, uint32_t y, Color color){
	if(y>=height || x>=width) return;
	screen[(y * width) + x].color = color;
}

void renderscreen(Pixel* screen, uint32_t width, uint32_t height){
	for(uint32_t i = 0; i < height * width; i++){
		Pixel pixel = screen[i];
		if(i % width == 0)
			printf("\n");
		printf("\033[38;2;%u;%u;%umo" RESET, pixel.color.r,pixel.color.g,pixel.color.b);
	}
}

void drawrectangle(Pixel* screen, uint32_t width, uint32_t height, Vec2 pos, Vec2 size, Color color){
	for(uint32_t y = pos.y; y < size.y; y++){
		for(uint32_t x = pos.x; x < size.x; x++){
			fillpixel(screen, width, height, x, y, color);
		}
	}
}

void drawtriangle(Pixel* screen, uint32_t width, uint32_t height, Vec2 vert1, Vec2 vert2, Vec2 vert3, Color color){
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
				fillpixel(screen, width, height, x, y, color);
			}
		}
	}
}
