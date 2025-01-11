#include "../project.h"
#include "../project.c"

int main() {

	Screen* screen = createscreen(10,10);

	clearscreen(screen, BLUE);

	drawrectangle(screen, (Vec2){0,0}, (Vec2){5,5}, RED);

	fillpixel(screen,3,3,WHITE);

	renderscreen(screen);

	free(screen);
}
