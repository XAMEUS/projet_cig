#include "ei_draw.h"
#include <stdlib.h>
#include <stdio.h>

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
/**
 * \brief	Copies a surface, or a subpart, to another one.
 *		The source and destination area of the copy (either the entire surfaces, or
 *		subparts) must have the same size (before clipping). Both the source and destination
 *		surfaces must be *locked* by \ref hw_surface_lock.
 *
 * @param	destination	The surface on which to copy pixels from the source surface.
 * @param	dst_rect	If NULL, the entire destination surface is used. If not NULL,
 *				defines the rectangle on the destination surface where to copy
 *				the pixels.
 * @param	source		The surface from which to copy pixels.
 * @param	src_rect	If NULL, the entire source surface is used. If not NULL, defines the
 *				rectangle on the source surface from which to copy the pixels.
 * @param	alpha		If true, the final pixels are a combination of source and
 *				destination pixels weighted by the source alpha channel. The
 *				transparency of the final pixels is set	to opaque.
 *				If false, the final pixels are an exact copy of the source pixels,
 				including the alpha channel.
 *
 * @return			Returns 0 on success, 1 on failure (different ROI size).
 */

int	ei_copy_surface(ei_surface_t destination,
						 const ei_rect_t* dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t* src_rect,
						 const ei_bool_t alpha) {
	uint32_t *src_buff = (uint32_t *) hw_surface_get_buffer(source);
 	ei_size_t src_size = hw_surface_get_size(source);
	uint32_t *dst_buff = (uint32_t *) hw_surface_get_buffer(destination);
	ei_size_t dst_size = hw_surface_get_size(destination);

	ei_point_t src_clipper_pt;
	ei_size_t src_clipper_size;
	if(src_rect) {
		if(src_rect->size.width >= 0 && src_rect->size.height >= 0) {
			src_clipper_pt.x = src_rect->top_left.x;
			src_clipper_pt.y = src_rect->top_left.y;
			src_clipper_size.width = src_rect->size.width;
			src_clipper_size.height = src_rect->size.height;
		}
		else return 1;
	} else {
		src_clipper_pt.x = 0;
		src_clipper_pt.y = 0;
		src_clipper_size.width = src_size.width;
		src_clipper_size.height = src_size.height;
	}
	ei_point_t dst_clipper_pt;
	ei_size_t clipper_size;
	if(dst_rect) {
		if(dst_rect->size.width >= 0 && dst_rect->size.height >= 0) {
			dst_clipper_pt.x = dst_rect->top_left.x;
			dst_clipper_pt.y = dst_rect->top_left.y;
			clipper_size.width = dst_rect->size.width;
			clipper_size.height = dst_rect->size.height;
		}
		else return 1;
	} else {
		dst_clipper_pt.x = 0;
		dst_clipper_pt.y = 0;
		clipper_size.width = dst_size.width;
		clipper_size.height = dst_size.height;
	}
	if(clipper_size.width == src_clipper_size.width &&
			clipper_size.height == src_clipper_size.height) {
		// Now we only use clipper_size because both have the same size
		if(src_clipper_pt.x < 0) {
			dst_clipper_pt.x -= src_clipper_pt.x;
			clipper_size.width -= src_clipper_pt.x;
			src_clipper_pt.x = 0;
		}
		if(src_clipper_pt.y < 0) {
			dst_clipper_pt.y -= src_clipper_pt.y;
			clipper_size.width -= src_clipper_pt.y;
			src_clipper_pt.y = 0;
		}
		if(src_clipper_pt.x + clipper_size.width >= src_size.width)
			clipper_size.width = src_size.width - src_clipper_pt.x;
		if(src_clipper_pt.y + clipper_size.height >= src_size.height)
			clipper_size.height = src_size.height - src_clipper_pt.y;
		if(dst_clipper_pt.x < 0) {
		    src_clipper_pt.x -= dst_clipper_pt.x;
		    clipper_size.width -= dst_clipper_pt.x;
		    dst_clipper_pt.x = 0;
		}
		if(dst_clipper_pt.y < 0) {
		    src_clipper_pt.y -= dst_clipper_pt.y;
		    clipper_size.width -= dst_clipper_pt.y;
		    dst_clipper_pt.y = 0;
		}
		if(dst_clipper_pt.x + clipper_size.width >= dst_size.width)
		    clipper_size.width = dst_size.width - dst_clipper_pt.x;
		if(dst_clipper_pt.y + clipper_size.height >= dst_size.height)
		    clipper_size.height = dst_size.height - dst_clipper_pt.y;

		if (clipper_size.width > 0 && clipper_size.height > 0) {
			// We can copy now
			int src_offset = src_clipper_pt.x * src_size.width + src_clipper_pt.y;
			int dst_offset = dst_clipper_pt.x * dst_size.width + dst_clipper_pt.y;
			for (int i = 0; i < clipper_size.height; i++)
				for (int j = 0; j < clipper_size.width; j++)
					dst_buff[i*dst_size.width + j + dst_offset] =
							src_buff[i*src_size.width + j + src_offset];
		}
		return 0;
	}
	else return 1;

}
