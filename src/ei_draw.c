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
	uint8_t *buff = hw_surface_get_buffer(surface);
	int W = hw_surface_get_size(surface).width;
	uint32_t col = ei_map_rgba(surface, &color);
	ei_point_t pt_min = first_point->point;
	ei_point_t pt_max = first_point->point;
	if(first_point->next != NULL) {
		ei_linked_point_t *ptr_tmp = (ei_linked_point_t *) first_point;
		while(ptr_tmp != NULL) {
			ei_point_t pt_0 = ptr_tmp->point;
			ei_point_t pt_1;
			pt_1 = (ptr_tmp->next != NULL) ? ptr_tmp->next->point : first_point->point;
			int dx = (pt_1.x > pt_0.x) ? pt_1.x - pt_0.x : pt_0.x - pt_1.x;
			int dy = (pt_1.y > pt_0.y) ? pt_1.y - pt_0.y : pt_0.y - pt_1.y;
			int incr_x = 1 - 2 * (pt_0.x >= pt_1.x);
			int incr_y = 1 - 2 * (pt_0.y >= pt_1.y);
			int E = dx - dy;
			while(pt_0.x != pt_1.x || pt_0.y != pt_1.y) {
				pt_min.x = (pt_min.x > pt_0.x) ? pt_min.x : pt_0.x;
				pt_min.y = (pt_min.y > pt_0.y) ? pt_min.y : pt_0.y;
				pt_max.x = (pt_max.x > pt_0.x) ? pt_max.x : pt_0.x;
				pt_max.y = (pt_max.y > pt_0.y) ? pt_max.y : pt_0.y;
				*((uint32_t*)buff + pt_0.x + W * pt_0.y) = col; //TODO clipper
				if(2 * E > - dy) {
					E -= dy;
					pt_0.x += incr_x;
				}
				if(2 * E < dx) {
					E += dx;
					pt_0.y += incr_y;
				}
			}
			ptr_tmp = ptr_tmp->next;
		}
		ei_size_t update_size = {pt_max.x - pt_min.x, pt_max.y - pt_min.y};
		ei_rect_t rect = {pt_min, update_size};
		ei_linked_rect_t linked_rect = {rect, NULL};
		hw_surface_update_rects(surface, &linked_rect);
		return;
	}
	if(clipper == NULL ||
			(first_point->point.x >= clipper->top_left.x &&
			first_point->point.y >= clipper->top_left.y &&
			first_point->point.x <= clipper->top_left.x + clipper->size.width &&
			first_point->point.y <= clipper->top_left.y + clipper->size.height)) {
		*((uint32_t*)buff + first_point->point.x + W * first_point->point.y) = col;
		ei_size_t update_size = {1, 1};
		ei_rect_t rect = {first_point->point, update_size};
		ei_linked_rect_t linked_rect = {rect, NULL};
		hw_surface_update_rects(surface, &linked_rect);
	}
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
    return;
}

int	ei_copy_surface(ei_surface_t destination,
						 const ei_rect_t* dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t* src_rect,
						 const ei_bool_t alpha) {
    return 0;
}
