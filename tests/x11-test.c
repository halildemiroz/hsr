#include <stdint.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <unistd.h>
#include <stdio.h>

#include "../project.h"
#include "../project.c"

int main() {
	int screennum;
	
	xcb_connection_t* conn = xcb_connect(NULL,&screennum);
	
	const xcb_setup_t* setup = xcb_get_setup(conn);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
	xcb_screen_t* screen = iter.data;

	xcb_drawable_t window = screen->root;
	xcb_gcontext_t foreground = xcb_generate_id(conn);
	uint32_t mask = XCB_GC_FOREGROUND;
	uint32_t value[2] = {screen->black_pixel, 0};

	xcb_create_gc(conn,foreground,window,mask,value);
	
	window = xcb_generate_id(conn);
	xcb_create_window(conn,
			XCB_COPY_FROM_PARENT,
			window,
			screen->root,
			0,0,
			800,600,
			0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			screen->root_visual,
			0,NULL);


	xcb_map_window(conn,window);
	xcb_flush(conn);

	xcb_generic_event_t* event;
	while((event = xcb_wait_for_event(conn))){
		switch (event->response_type & ~0x80) {
			case XCB_EXPOSE: {
				xcb_expose_event_t* exposure = (xcb_expose_event_t*)event;
				printf("expose: %i, %i\n",exposure->width, exposure->height);
			}
			default: break;
		}
	}

	xcb_disconnect(conn);
}

