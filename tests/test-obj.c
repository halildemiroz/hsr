#include "../project.h"
#include "../project.c"

int main() {
	uint32_t width = 20, height = 20;
	Pixel* screen = malloc(sizeof(Pixel) * width * height);
	
	clearscreen(screen, width, height, BLUE);
	drawrectangle(screen, width, height, (Vec2){0,0}, (Vec2){5,5}, RED);

	renderscreen(screen, width, height);

	free(screen);
}
