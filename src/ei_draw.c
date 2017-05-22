#include "ei_draw.h"
#include <stdlib.h>

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color) {
    int r, g, b, a;
    hw_surface_get_channel_indices(surface, &r, &g, &b, &a);
    uint8_t d = a < 0;
    uint32_t c = (color->red << (8*(3-r-d))) +
                    (color->green << (8*(3-g-d))) +
                    (color->blue << (8*(3-b-d))) +
                    ((a >= 0) ? (color->alpha << (8*(3-a))) : 0);
    return c;
}

void ei_draw_polyline (ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) {
    if(first_point == NULL) return;
    if(first_point->next == NULL) {
        //Dessiner
        return;
    }
    ei_linked_point_t *a = (ei_linked_point_t *) first_point;
    do {
        int x_1 = a->point.x;
        int y_1 = a->point.y;
        int x_2 = a->next->point.x;
        int y_2 = a->next->point.y;
        int abs_dx = abs(x_2 - x_1);
        int abs_dy = abs(y_2 - y_1);
        int incr_x = 1 - 2 * (x_2 < x_1);
        int incr_y = 1 - 2 * ((y_2 > y_1) ^ (abs_dx < abs_dy));
        if(abs_dx > abs_dy) {
            int x = x_1;
            int y = y_1;
            int E = 0;
            while(x != x_2) {
                //Dessiner(x,y)
                x += incr_x;
                E += abs_dy;
                if(2 * E > abs_dx) {
                    y += incr_y;
                    E -= abs_dx;
                }
            }
        }
        else {
            int y = y_1;
            int x = x_1;
            int E = 0;
            while(y != y_2) {
                //Dessiner(x,y)
                y += incr_y;
                E += abs_dx;
                if(2 * E > abs_dy) {
                    x += incr_x;
                    E -= abs_dy;
                }
            }
        }
    a = a->next;
    } while(a->next != NULL && a->next != first_point);
}
void ei_draw_polygon(ei_surface_t surface,
						 const ei_linked_point_t* first_point,
						 const ei_color_t color,
						 const ei_rect_t* clipper) {
    return;
}

void ei_draw_text (ei_surface_t	surface,
						 const ei_point_t* where,
						 const char* text,
						 const ei_font_t font,
						 const ei_color_t* color,
						 const ei_rect_t* clipper) {
    return;
}

void ei_fill (ei_surface_t surface,
						 const ei_color_t* color,
						 const ei_rect_t* clipper) {
    return;
}

int	ei_copy_surface(ei_surface_t destination,
						 const ei_rect_t* dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t* src_rect,
						 const ei_bool_t alpha) {
    return 0;
}
