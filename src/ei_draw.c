#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "ei_draw.h"


uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color) {
    int r, g, b, a;
    hw_surface_get_channel_indices(surface, &r, &g, &b, &a);
    uint32_t c = 255;
    uint8_t *composante = (uint8_t*) &c;
    composante[r] = color->red;
    composante[g] = color->green;
    composante[b] = color->blue;
    if(a >= 0) composante[a] = color->alpha;
    return c;
}

void ei_draw_polyline (ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) {
    if(first_point == NULL) return;
	ei_point_t pt_min = first_point->point;
	ei_point_t pt_max = first_point->point;
	ei_linked_point_t *ptr_tmp = (ei_linked_point_t *) first_point;
	while(ptr_tmp != NULL) {
		pt_min.x = (pt_min.x < ptr_tmp->point.x) ? pt_min.x : ptr_tmp->point.x;
		pt_min.y = (pt_min.y < ptr_tmp->point.y) ? pt_min.y : ptr_tmp->point.y;
		pt_max.x = (pt_max.x > ptr_tmp->point.x) ? pt_max.x : ptr_tmp->point.x;
		pt_max.y = (pt_max.y > ptr_tmp->point.y) ? pt_max.y : ptr_tmp->point.y;
		ptr_tmp = ptr_tmp->next;
	}
	ei_size_t box = {pt_max.x - pt_min.x + 1, pt_max.y - pt_min.y + 1};
	ei_surface_t n_surface = hw_surface_create(surface, &box, EI_TRUE);
	uint32_t col = ei_map_rgba(n_surface, &color);
	hw_surface_lock(n_surface);
	uint32_t *n_buff = (uint32_t *) hw_surface_get_buffer(n_surface);
	int offset = pt_min.x + pt_min.y * box.width;

	ptr_tmp = (ei_linked_point_t *) first_point;
	while(ptr_tmp != NULL) {
		ei_point_t pt_0 = ptr_tmp->point;
		ei_point_t pt_1;
		pt_1 = (ptr_tmp->next != NULL) ? ptr_tmp->next->point : first_point->point;
		int dx = (pt_1.x > pt_0.x) ? pt_1.x - pt_0.x : pt_0.x - pt_1.x;
		int dy = (pt_1.y > pt_0.y) ? pt_1.y - pt_0.y : pt_0.y - pt_1.y;
		int incr_x = 1 - 2 * (pt_0.x >= pt_1.x);
		int incr_y = 1 - 2 * (pt_0.y >= pt_1.y);
		int E = dx - dy;
		do {
			n_buff[pt_0.x + box.width * pt_0.y - offset] = col;
			if(2 * E > - dy) {
				E -= dy;
				pt_0.x += incr_x;
			}
			if(2 * E < dx) {
				E += dx;
				pt_0.y += incr_y;
			}
		} while(pt_0.x != pt_1.x || pt_0.y != pt_1.y);
		ptr_tmp = ptr_tmp->next;
	}
    ei_point_t pt_src = {0, 0};
    ei_point_t pt_dst = pt_min;
    ei_size_t draw_box = box;
    if(clipper) {
        if(clipper->top_left.x > pt_dst.x) {
            draw_box.width += clipper->top_left.x - pt_dst.x;
            pt_src.x += clipper->top_left.x - pt_dst.x;
            pt_dst.x = clipper->top_left.x;
        }
        if(clipper->top_left.y > pt_dst.y) {
            draw_box.height += clipper->top_left.y - pt_dst.y;
            pt_src.y += clipper->top_left.y - pt_dst.y;
            pt_dst.y = clipper->top_left.y;
        }
        //Not a bug: ei_copy_surface will cut the box to stay into the drawing area
        draw_box.width = clipper->top_left.x - pt_dst.x + clipper->size.width;
        draw_box.height = clipper->top_left.y - pt_dst.y + clipper->size.height;
    }
    ei_rect_t dst_rect = {pt_dst, draw_box};
    ei_rect_t src_rect = {pt_src, draw_box};
	ei_copy_surface(surface, &dst_rect, n_surface, &src_rect, EI_TRUE);
	hw_surface_unlock(n_surface);
	hw_surface_free(n_surface);
}

void ei_draw_text(ei_surface_t	surface,
					const ei_point_t* where,
					const char* text,
					const ei_font_t font,
					const ei_color_t* color,
					const ei_rect_t* clipper) {
	ei_surface_t text_surface = hw_text_create_surface(text, font, color);
    ei_rect_t clipper_text = hw_surface_get_rect(text_surface);
    ei_point_t pt_src = {0, 0};
    ei_point_t pt_dst = *where;
    ei_size_t draw_box = clipper_text.size;
    if (clipper) {
        if(clipper->top_left.x > pt_dst.x) {
            draw_box.width += clipper->top_left.x - pt_dst.x;
            pt_src.x += clipper->top_left.x - pt_dst.x;
            pt_dst.x = clipper->top_left.x;
        }
        if(clipper->top_left.y > pt_dst.y) {
            draw_box.height += clipper->top_left.y - pt_dst.y;
            pt_src.y += clipper->top_left.y - pt_dst.y;
            pt_dst.y = clipper->top_left.y;
        }
        //Not a bug: ei_copy_surface will cut the box to stay into the drawing area
        draw_box.width = clipper->top_left.x - pt_dst.x + clipper->size.width;
        draw_box.height = clipper->top_left.y - pt_dst.y + clipper->size.height;
    }
    ei_rect_t dst_rect = {pt_dst, draw_box};
    ei_rect_t src_rect = {pt_src, draw_box};
    ei_copy_surface(surface, &dst_rect, text_surface, &src_rect, EI_TRUE);
    hw_surface_free(text_surface);
}

void ei_fill(ei_surface_t surface,
                const ei_color_t* color,
                const ei_rect_t* clipper) {
    uint32_t *p = (uint32_t *)hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    ei_color_t black = {0, 0, 0, 255};
    uint32_t mcolor = (color) ? ei_map_rgba(surface, color) : ei_map_rgba(surface, &black);
    if (clipper == NULL) {
        if (color && color->alpha != 255) {
            int r, g, b, a;
            hw_surface_get_channel_indices(surface, &r, &g, &b, &a);
            int c = 0;
            while (c < size.width * size.height) {
                uint8_t* d_pos = (uint8_t*) p;
                uint8_t s_r = color->red;
                uint8_t s_g = color->green;
                uint8_t s_b = color->blue;
                uint8_t s_a = color->alpha;
                uint8_t d_r = d_pos[r];
                uint8_t d_g = d_pos[g];
                uint8_t d_b = d_pos[b];
                d_pos[r] = ((uint16_t) d_r * (255 - s_a) + (uint16_t) s_r * s_a) / 255;
                d_pos[g] = ((uint16_t) d_g * (255 - s_a) + (uint16_t) s_g * s_a) / 255;
                d_pos[b] = ((uint16_t) d_b * (255 - s_a) + (uint16_t) s_b * s_a) / 255;
                if(a >= 0) d_pos[a] = 255;
                p++;
                c++;
            }
        } else {
            int c = 0;
            while (c < size.width * size.height) {
                *p = mcolor;
                p++;
                c++;
            }
        }
    }
	else {
		ei_rect_t n_clipper = *clipper;
		if(clipper->top_left.x < 0) {
			n_clipper.size.width = clipper->size.width + clipper->top_left.x;
			n_clipper.top_left.x = 0;
		}
		if(clipper->top_left.y < 0) {
			n_clipper.size.height = clipper->size.height + clipper->top_left.y;
			n_clipper.top_left.y = 0;
		}
		if(n_clipper.top_left.x + n_clipper.size.width >= size.width)
			n_clipper.size.width = size.width - n_clipper.top_left.x;
		if(n_clipper.top_left.y + n_clipper.size.height >= size.height)
			n_clipper.size.height = size.height - n_clipper.top_left.y;

        int x; int y = 0;
        p += size.width * n_clipper.top_left.y + n_clipper.top_left.x;
        uint32_t dx = n_clipper.top_left.x + size.width - (n_clipper.top_left.x + n_clipper.size.width);

        if (color && color->alpha != 255) {
            int r, g, b, a;
            hw_surface_get_channel_indices(surface, &r, &g, &b, &a);
            while (y < n_clipper.size.height) {
        		x = 0;
        		while (x < n_clipper.size.width) {
					uint8_t* d_pos = (uint8_t*) p;
					uint8_t s_r = color->red;
					uint8_t s_g = color->green;
					uint8_t s_b = color->blue;
					uint8_t s_a = color->alpha;
					uint8_t d_r = d_pos[r];
					uint8_t d_g = d_pos[g];
					uint8_t d_b = d_pos[b];
					d_pos[r] = ((uint16_t) d_r * (255 - s_a) + (uint16_t) s_r * s_a) / 255;
					d_pos[g] = ((uint16_t) d_g * (255 - s_a) + (uint16_t) s_g * s_a) / 255;
					d_pos[b] = ((uint16_t) d_b * (255 - s_a) + (uint16_t) s_b * s_a) / 255;
                    if(a >= 0) d_pos[a] = 255;
        			p++; x++;
			    }
			    p += dx; y++;
            }
        } else {
            while (y < n_clipper.size.height) {
        		x = 0;
        		while (x < n_clipper.size.width) {
        			*p = mcolor;
        			p++; x++;
			    }
			    p += dx; y++;
            }
        }
	}
}

void print_linked_point(ei_linked_point_t* first_point) {
    int i = 0;
    for (ei_linked_point_t* p = first_point; p != NULL; p = p->next) {
        fprintf(stderr, "point n°%d : %d %d \n", i, p->point.x, p->point.y);
        i++;
    }
}

int	ei_copy_surface(ei_surface_t destination,
						 const ei_rect_t* dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t* src_rect,
						 const ei_bool_t alpha) {
 	ei_size_t src_size = hw_surface_get_size(source);
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
			clipper_size.width += src_clipper_pt.x;
			src_clipper_pt.x = 0;
		}
		if(src_clipper_pt.y < 0) {
			dst_clipper_pt.y -= src_clipper_pt.y;
			clipper_size.width += src_clipper_pt.y;
			src_clipper_pt.y = 0;
		}
		if(src_clipper_pt.x + clipper_size.width >= src_size.width)
			clipper_size.width = src_size.width - src_clipper_pt.x;
		if(src_clipper_pt.y + clipper_size.height >= src_size.height)
			clipper_size.height = src_size.height - src_clipper_pt.y;
		if(dst_clipper_pt.x < 0) {
		    src_clipper_pt.x -= dst_clipper_pt.x;
		    clipper_size.width += dst_clipper_pt.x;
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
			int src_offset = src_clipper_pt.y * src_size.width + src_clipper_pt.x;
			int dst_offset = dst_clipper_pt.y * dst_size.width + dst_clipper_pt.x;
			if (alpha) {
				uint8_t *src_buff = hw_surface_get_buffer(source);
				uint8_t *dst_buff = hw_surface_get_buffer(destination);
			    int r, g, b, a;
			    hw_surface_get_channel_indices(destination, &r, &g, &b, &a);
			    int r1, g1, b1, a1;
			    hw_surface_get_channel_indices(source, &r1, &g1, &b1, &a1);
				for (int i = 0; i < clipper_size.height; i++) {
                    int s_i = i * src_size.width;
                    int d_i = i * dst_size.width;
					for (int j = 0; j < clipper_size.width; j++) {
						uint8_t* s_pos = src_buff + ((s_i + j + src_offset) * 4);
						uint8_t* d_pos = dst_buff + ((d_i + j + dst_offset) * 4);
						uint8_t s_r = s_pos[r1];
						uint8_t s_g = s_pos[g1];
						uint8_t s_b = s_pos[b1];
						uint8_t s_a = s_pos[a1];
						uint8_t d_r = d_pos[r];
						uint8_t d_g = d_pos[g];
						uint8_t d_b = d_pos[b];
						d_pos[r] = ((uint16_t) d_r * (255 - s_a) + (uint16_t) s_r * s_a) / 255;
						d_pos[g] = ((uint16_t) d_g * (255 - s_a) + (uint16_t) s_g * s_a) / 255;
						d_pos[b] = ((uint16_t) d_b * (255 - s_a) + (uint16_t) s_b * s_a) / 255;
                        if(a >= 0) d_pos[a] = 255;
					}
				}
			}
			else {
				uint32_t *src_buff = (uint32_t *) hw_surface_get_buffer(source);
				uint32_t *dst_buff = (uint32_t *) hw_surface_get_buffer(destination);
				for (int i = 0; i < clipper_size.height; i++) {
					for (int j = 0; j < clipper_size.width; j++)
						dst_buff[i*dst_size.width + j + dst_offset] =
						            src_buff[i*src_size.width + j + src_offset];
				}
			}
		}
		return 0;
	}
	else return 1;

}
