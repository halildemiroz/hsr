#include "../hsr.h"
#include "../hsr.c"

int main() {

	Screen* screen = createscreen(20,20);

	clearscreen(screen, RED);

	//drawrectangle(screen, (Vec2){5,5}, (Vec2){10,10}, RED);

	//fillpixel(screen,3,3,WHITE);
	
	drawcircle(screen, (Vec2){3,3}, 5, BLUE);

	renderscreen(screen);

	free(screen);
}
