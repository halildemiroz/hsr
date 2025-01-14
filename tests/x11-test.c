#include <stdint.h>
#include <xcb/xcb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xproto.h>

#include "../hsr.h"
#include "../hsr.c"

uint32_t rgbtohex(Color color){
    return ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
}

int main() {

    Screen* pixels = createscreen(500, 500);

    xcb_connection_t* conn = xcb_connect(NULL, NULL);
    
    // Screen
    const xcb_setup_t* setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t* screen = iter.data;

    // Graphing context
    xcb_drawable_t window = screen->root;
    xcb_gcontext_t gc = xcb_generate_id(conn);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t vals[] = {
        screen->white_pixel,
        0
    };

    xcb_create_gc(conn,
            gc,
            window,
            mask,
            vals);
    
    // Window
    uint32_t valwin[] = {screen->white_pixel,
                                    XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
                                    XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                                    XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
                                    XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE };
    window = xcb_generate_id(conn);
    
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    
    xcb_create_window(
            conn,
            XCB_COPY_FROM_PARENT,
            window,
            screen->root,
            0,0,
            pixels->width,pixels->height,
            1,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            screen->root_visual,
            mask,valwin
            );

    xcb_map_window(conn, window);
    xcb_flush(conn);

    clearscreen(pixels, BLACK);

    // DRAWING IS HEREEEEEE ------------------------------
    
    /*
    drawrectangle(pixels, (Vec2){70,70}, (Vec2){100,100}, GREEN);
    drawtriangle(pixels, (Vec2){100,100}, (Vec2){250,100}, (Vec2){250,250}, BLUE);
    drawtriangle(pixels, (Vec2){250,250}, (Vec2){100,250}, (Vec2){100,100}, RED);
    */
    
    drawcircle(pixels, (Vec2){100,100}, 100, LILA);
    // ---------------------------------------------------

    xcb_generic_event_t* event;
    while((event = xcb_wait_for_event(conn))){
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE:{
                xcb_expose_event_t* expose = (xcb_expose_event_t*)event;
                printf("expose: %i, %i\n", expose->width, expose->height);

                int32_t x = 0, y = 0;
                for(uint32_t i = 0; i < pixels->width * pixels->height; i++){
                    if(i % pixels->width == 0){
                        x = 0;
                        y++;
                    }
                    Pixel pixel = pixels->pixels[i];
                    
                    xcb_point_t point[] = { {x,y} };
                    uint32_t vals[] = {rgbtohex(pixel.color)};

                    xcb_change_gc(conn, gc, XCB_GC_FOREGROUND, vals);
                    xcb_poly_point(conn,XCB_COORD_MODE_ORIGIN, window,gc,1,point);
                    x++;
                }
                xcb_flush(conn);
                break;
            }
            case XCB_KEY_PRESS:{
                xcb_button_press_event_t* press = (xcb_button_press_event_t*)event;
                printf("Key pressed: %i\n", press->detail);

                switch (press->detail){
                    case 38:{
                        drawrectangle(pixels, (Vec2){0,0}, (Vec2){70,70}, GREEN);
                        int32_t x = 0, y = 0;
                        for(uint32_t i = 0; i < pixels->width * pixels->height; i++){
                              if(i % pixels->width == 0){
                                    x = 0;
                                    y++;
                                }
                        Pixel pixel = pixels->pixels[i];
                    
                        xcb_point_t point[] = { {x,y} };
                        uint32_t vals[] = {rgbtohex(pixel.color)};

                        xcb_change_gc(conn, gc, XCB_GC_FOREGROUND, vals);
                        xcb_poly_point(conn,XCB_COORD_MODE_ORIGIN, window,gc,1,point);
                        x++;
                        }
                        xcb_flush(conn);
                        break;
                    }

                    case 39:{
                        drawrectangle(pixels, (Vec2){250,250}, (Vec2){400,400}, RED);
                        int32_t x = 0, y = 0;
                        for(uint32_t i = 0; i < pixels->width * pixels->height; i++){
                            if(i % pixels->width == 0){
                                x = 0;
                                y++;
                            }
                            Pixel pixel = pixels->pixels[i];
                    
                            xcb_point_t point[] = { {x,y} };
                            uint32_t vals[] = {rgbtohex(pixel.color)};

                            xcb_change_gc(conn, gc, XCB_GC_FOREGROUND, vals);
                            xcb_poly_point(conn,XCB_COORD_MODE_ORIGIN, window,gc,1,point);
                            x++;
                        }       
                        xcb_flush(conn);
                        break;
                    }

                    case 40:{
                        drawrectangle(pixels, (Vec2){400,400}, (Vec2){500,500}, BLUE);
                        int32_t x = 0, y = 0;
                        for(uint32_t i = 0; i < pixels->width * pixels->height; i++){
                        if(i % pixels->width == 0){
                            x = 0;
                            y++;
                            }
                        Pixel pixel = pixels->pixels[i];
                    
                        xcb_point_t point[] = { {x,y} };
                        uint32_t vals[] = {rgbtohex(pixel.color)};

                        xcb_change_gc(conn, gc, XCB_GC_FOREGROUND, vals);
                        xcb_poly_point(conn,XCB_COORD_MODE_ORIGIN, window,gc,1,point);
                        x++;
                    }
                        xcb_flush(conn);
                        break;
                    }
                default:
                    break;
                }

                break;
            }
            default: break;
        }
    }

    xcb_disconnect(conn);    
    return 0;
}
