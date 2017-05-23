#include "ei_draw.h"
#include <stdlib.h>
#include <stdio.h>

void private_ei_draw_line (ei_surface_t			surface,
						 ei_point_t           	a,
                         ei_point_t             b,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper);

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
void private_ei_draw_line (ei_surface_t			surface,
						 ei_point_t           	pt_0,
                         ei_point_t             pt_1,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) {
	int dx = abs(pt_1.x - pt_0.x);
    int dy = abs(pt_1.y - pt_0.y);
	int incr_x = 1 - 2 * (pt_0.x >= pt_1.x);
    int incr_y = 1 - 2 * (pt_0.y >= pt_1.y);
	int E = dx - dy;
	//Dessiner(x, y)
	while(pt_0.x != pt_1.x || pt_0.y != pt_1.y) {
		printf("=>%d %d %d %d\n", pt_0.x, pt_0.y, pt_1.y, pt_1.y);
		//Dessiner(x, y)
		if(2 * E > - dy) {
			E -= dy;
			pt_0.x += incr_x;
		}
		// if(pt_0.x == pt_1.x && pt_0.y == pt_1.y) break;
		if(2 * E < dx) {
			E += dx;
			pt_0.y += incr_y;
		}
	}
// 	int abs_dx = abs(b.x - a.x);
//     int abs_dy = abs(b.y - a.y);
//     int incr_x = 1 - 2 * (b.x < a.x);
//     int incr_y = 1 - 2 * ((b.y > a.y) ^ (abs_dx < abs_dy));
//     if(abs_dx > abs_dy) {
//         int x = a.x;
//         int y = a.y;
//         int E = 0;
//         while(x != b.x) {
//             printf("");
//             //Dessiner(x,y)
//             x += incr_x;
//             E += abs_dy;
//             if(2 * E > abs_dx) {
//                 y += incr_y;
//                 E -= abs_dx;
//             }
//         }
//     }
//     else {
//         int y = a.y;
//         int x = a.x;
//         int E = 0;
//         while(y != b.y) {
//             printf("%d, %d\n", abs_dx, abs_dy);
//             //Dessiner(x,y)
//             y += incr_y;
//             E += abs_dx;
//             if(2 * E > abs_dy) {
//                 x += incr_x;
//                 E -= abs_dy;
//             }
//         }
//     }
}

/**
 * \brief	Draws a line made of many line segments.
 *
 * @param	surface 	Where to draw the line. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	first_point 	The head of a linked list of the points of the line. It can be NULL
 *				(i.e. draws nothing), can have a single point, or more.
 *				If the last point is the same as the first point, then this pixel is
 *				drawn only once.
 * @param	color		The color used to draw the line, alpha channel is managed.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_polyline (ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) {
    if(first_point == NULL) return;
    if(first_point->next == NULL) {
        return;
    }
    ei_linked_point_t *ptr_tmp = (ei_linked_point_t *) first_point;
    while(ptr_tmp->next != NULL) {
        private_ei_draw_line(surface, ptr_tmp->point, ptr_tmp->next->point, color, clipper);
        ptr_tmp = ptr_tmp->next;
    }
    private_ei_draw_line(surface, ptr_tmp->point, first_point->point, color, clipper);
}
void ei_draw_polygon(ei_surface_t surface,
						 const ei_linked_point_t* first_point,
						 const ei_color_t color,
						 const ei_rect_t* clipper) {
    return;
}

void ei_draw_text(ei_surface_t	surface,
					const ei_point_t* where,
					const char* text,
					const ei_font_t font,
					const ei_color_t* color,
					const ei_rect_t* clipper) {
	ei_surface_t text_surface = hw_text_create_surface(text, font, color);
	ei_copy_surface(surface, clipper, text_surface, clipper, EI_TRUE);
    return;
}

void ei_fill(ei_surface_t surface,
                const ei_color_t* color,
                const ei_rect_t* clipper) {
    uint32_t *p = (uint32_t *)hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    if (clipper == NULL) {
        int c = 0;
        while (c < size.width * size.height) {
            *p = ei_map_rgba(surface, color);
            p++;
            c++;
        }
    }
    else {
        p += size.width * clipper->top_left.y + clipper->top_left.x;
        int x; int y = 0;
        uint32_t dx = clipper->top_left.x +
                        size.width - (clipper->top_left.x + clipper->size.width);
        while (y < clipper->size.height) {
            x = 0;
            while (x < clipper->size.width) {
                *p = ei_map_rgba(surface, color);
                p++; x++;
            }
            p += dx; y++;
        }
    }
    return;
}

int	ei_copy_surface(ei_surface_t destination,
						 const ei_rect_t* dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t* src_rect,
						 const ei_bool_t alpha) {
    return 0;
}
