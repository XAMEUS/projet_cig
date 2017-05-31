#include "ei_draw.h"
#include "ei_draw_ex.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

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
    if (clipper == NULL) {
        int c = 0;
        while (c < size.width * size.height) {
            *p = ei_map_rgba(surface, color);
            p++;
            c++;
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
			n_clipper.size.width = n_clipper.size.width - n_clipper.top_left.x;
		if(n_clipper.top_left.y + n_clipper.size.height >= size.height)
			n_clipper.size.height = n_clipper.size.height - n_clipper.top_left.y;
		p += size.width * n_clipper.top_left.y + n_clipper.top_left.x;
		int x; int y = 0;
		uint32_t dx = n_clipper.top_left.x +
		size.width - (n_clipper.top_left.x + n_clipper.size.width);
		while (y < n_clipper.size.height) {
			x = 0;
			while (x < n_clipper.size.width) {
				*p = ei_map_rgba(surface, color);
				p++; x++;
			}
			p += dx; y++;
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
					for (int j = 0; j < clipper_size.width; j++) {
						int s_pos = (i*src_size.width + j + src_offset) * 4;
						int d_pos = (i*dst_size.width + j + dst_offset) * 4;
						uint8_t s_r = src_buff[s_pos + r1];
						uint8_t s_g = src_buff[s_pos + g1];
						uint8_t s_b = src_buff[s_pos + b1];
						uint8_t s_a = src_buff[s_pos + a1];
						uint8_t d_r = dst_buff[d_pos + r];
						uint8_t d_g = dst_buff[d_pos + g];
						uint8_t d_b = dst_buff[d_pos + b];
						dst_buff[d_pos + r] = ((uint16_t) d_r * (255 - s_a) + (uint16_t) s_r * s_a) / 255;
						dst_buff[d_pos + g] = ((uint16_t) d_g * (255 - s_a) + (uint16_t) s_g * s_a) / 255;
						dst_buff[d_pos + b] = ((uint16_t) d_b * (255 - s_a) + (uint16_t) s_b * s_a) / 255;
                        if(a >= 0) dst_buff[d_pos + a] = 255;
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

/* draw button */
ei_linked_point_t* arc(ei_linked_point_t** first_point,
                       const ei_point_t center,
                       const int radius,
                       const float first_angle,
                       const float last_angle) {
    int step_nbr = radius; // a reflechir
    float angle_step = (last_angle - first_angle) / step_nbr;

    *first_point = malloc(sizeof(ei_linked_point_t));
    assert(*first_point != NULL);
    (*first_point)->point.x = center.x + radius * cos(first_angle);
    (*first_point)->point.y = center.y - radius * sin(first_angle);
    (*first_point)->next = NULL;
    // int i = 0;
    // fprintf(stderr, "point n°%d : %d %d \n", i, (*first_point)->point.x, (*first_point)->point.y);
    ei_linked_point_t* last_point = *first_point;
    for(float angle = (first_angle + angle_step); radius != 0 && angle < last_angle; angle += angle_step) {
        ei_linked_point_t* point = malloc(sizeof(ei_linked_point_t));
        assert(point != NULL);
        point->point.x = center.x + radius * cos(angle);
        point->point.y = center.y - radius * sin(angle);
        last_point->next = point;
        last_point = last_point->next;
        // i++;
        // fprintf(stderr, "point n°%d : %d %d %f\n", i, point->point.x, point->point.y, angle);
    }
    last_point->next = NULL;
    return last_point;
}

ei_linked_point_t* rounded_frame(ei_rect_t frame,
                                 int radius) {
    /* top left */
    ei_point_t center = {frame.top_left.x + radius, frame.top_left.y + radius};
    ei_linked_point_t* first_top_left = NULL;
    ei_linked_point_t* last_top_left = arc(&first_top_left, center, radius, M_PI / 2, M_PI);
    /* bottom left */
    center.y += frame.size.height - 2 * radius;
    ei_linked_point_t* first_bottom_left = NULL;
    ei_linked_point_t* last_bottom_left = arc(&first_bottom_left, center, radius, M_PI, 3 * M_PI / 2);
    last_top_left->next = first_bottom_left;
    /* bottom right */
    center.x += frame.size.width - 2 * radius;
    ei_linked_point_t* first_bottom_right = NULL;
    ei_linked_point_t* last_bottom_right = arc(&first_bottom_right, center, radius, 3 * M_PI / 2, 2 * M_PI);
    last_bottom_left->next = first_bottom_right;
    /* top right */
    center.y = frame.top_left.y + radius;
    ei_linked_point_t* first_top_right = NULL;
    ei_linked_point_t* last_top_right = arc(&first_top_right, center, radius, 0, M_PI / 2);
    last_bottom_right->next = first_top_right;
    /* final link */
    ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
    assert(last_point != NULL);
    last_point->point.x = first_top_left->point.x;
    last_point->point.y = first_top_left->point.y;
    last_top_right->next = last_point;
    last_point->next = NULL;

    return first_top_left;
}

ei_linked_point_t* up_rounded_frame(ei_rect_t frame,
                                    int radius,
                                    int h) {
    /* top left */
    ei_point_t center = {frame.top_left.x + radius, frame.top_left.y + radius};
    ei_linked_point_t* first_top_left = NULL;
    ei_linked_point_t* last_top_left = arc(&first_top_left, center, radius, M_PI / 2, M_PI);
    /* bottom left */
    center.y += frame.size.height - 2 * radius;
    ei_linked_point_t* first_bottom_left = NULL;
    ei_linked_point_t* last_bottom_left = arc(&first_bottom_left, center, radius, M_PI, 5 * M_PI / 4);
    last_top_left->next = first_bottom_left;
    /* zigzag */
    ei_linked_point_t* point = malloc(sizeof(ei_linked_point_t));
    assert(point != NULL);
    point->point.x = frame.top_left.x + h;
    point->point.y = frame.top_left.y + frame.size.height - h;
    last_bottom_left->next = point;
    ei_linked_point_t* next_point = malloc(sizeof(ei_linked_point_t));
    assert(next_point != NULL);
    next_point->point.x = frame.top_left.x + frame.size.width - h;
    next_point->point.y = point->point.y;
    point->next = next_point;
    /* top right */
    center.x += frame.size.width - 2 * radius;
    center.y = frame.top_left.y + radius;
    ei_linked_point_t* first_top_right = NULL;
    ei_linked_point_t* last_top_right = arc(&first_top_right, center, radius, M_PI / 4, M_PI / 2);
    next_point->next = first_top_right;
    /* final link */
    ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
    assert(last_point != NULL);
    last_point->point.x = first_top_left->point.x;
    last_point->point.y = first_top_left->point.y;
    last_top_right->next = last_point;
    last_point->next = NULL;

    return first_top_left;
}

ei_linked_point_t* down_rounded_frame(ei_rect_t frame,
                                      int radius,
                                      int h) {
    /* bottom right */
    ei_point_t center = {frame.top_left.x + frame.size.width - radius, frame.top_left.y + frame.size.height - radius};
    ei_linked_point_t* first_bottom_right = NULL;
    ei_linked_point_t* last_bottom_right = arc(&first_bottom_right, center, radius, 3 * M_PI / 2, 2 * M_PI);
    /* top right */
    center.y = frame.top_left.y + radius;
    ei_linked_point_t* first_top_right = NULL;
    ei_linked_point_t* last_top_right = arc(&first_top_right, center, radius, 0, M_PI / 4);
    last_bottom_right->next = first_top_right;
    /* zigzag */
    ei_linked_point_t* point = malloc(sizeof(ei_linked_point_t));
    assert(point != NULL);
    point->point.x = frame.top_left.x + frame.size.width - h;
    point->point.y = frame.top_left.y + frame.size.height - h;
    last_top_right->next = point;
    ei_linked_point_t* next_point = malloc(sizeof(ei_linked_point_t));
    assert(next_point != NULL);
    next_point->point.x = frame.top_left.x + h;
    next_point->point.y = point->point.y;
    point->next = next_point;
    /* bottom left */
    center.x = frame.top_left.x + radius;
    center.y += frame.size.height - 2 * radius;
    ei_linked_point_t* first_bottom_left = NULL;
    ei_linked_point_t* last_bottom_left = arc(&first_bottom_left, center, radius, 5 * M_PI / 4, 3 * M_PI / 2);
    next_point->next = first_bottom_left;
    /* final link */
    ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
    assert(last_point != NULL);
    last_point->point.x = first_bottom_right->point.x;
    last_point->point.y = first_bottom_right->point.y;
    last_bottom_left->next = last_point;
    last_point->next = NULL;

    return first_bottom_right;
}

void free_linked_point(ei_linked_point_t* first_point) {
    ei_linked_point_t* next_point;
    while (first_point != NULL) {
        next_point = first_point->next;
        free(first_point);
        first_point = next_point;
    }
}

void ei_draw_button(ei_surface_t surface,
                    ei_rect_t* clipper,
                    ei_rect_t frame,
                    int radius,
                    int border,
                    ei_color_t color,
                    ei_relief_t relief,
                    ei_bool_t push){
    if (border && relief != ei_relief_none) {
        int min = (frame.size.height < frame.size.width) ? frame.size.height : frame.size.width;
        assert(border <= min);
        float factor = 0.1;
        ei_color_t shade = {color.red * (1 - factor),
                            color.green * (1 - factor),
                            color.blue * (1 - factor),
                            color.alpha};
        ei_color_t tint = {color.red + (255 - color.red) * factor,
                           color.green + (255 - color.green) * factor,
                           color.blue + (255 - color.blue) * factor,
                           color.alpha};
        if (relief == ei_relief_sunken) {
            ei_color_t tmp = shade;
            shade = tint;
            tint = tmp;
        }
        ei_linked_point_t* up_pts = up_rounded_frame(frame, radius, frame.size.height/2);
        ei_linked_point_t* down_pts = down_rounded_frame(frame, radius, frame.size.height/2);
        if (push) {
            ei_draw_polygon(surface, up_pts, shade, clipper);
            ei_draw_polygon(surface, down_pts, tint, clipper);
        } else {
            ei_draw_polygon(surface, up_pts, tint, clipper);
            ei_draw_polygon(surface, down_pts, shade, clipper);
        }
        free_linked_point(up_pts);
        free_linked_point(down_pts);
    }

    frame.size.width -= 2 * border;
    frame.size.height -= 2 * border;
    frame.top_left.x +=  border;
    frame.top_left.y += border;
    if (radius) {
        ei_linked_point_t* all_pts = rounded_frame(frame, radius);
        ei_draw_polygon(surface, all_pts, color, clipper);
        free_linked_point(all_pts);
    } else ei_fill(surface, &color, &frame);
}

/* draw toplevel */
ei_linked_point_t* toplevel_frame(ei_rect_t frame) {
    int radius = 10;
    /* top left */
    ei_point_t center = {frame.top_left.x + radius, frame.top_left.y + radius};
    ei_linked_point_t* first_top_left = NULL;
    ei_linked_point_t* last_top_left = arc(&first_top_left, center, radius, M_PI/2, M_PI);
    /* bottom left */
    ei_linked_point_t* bottom_left = malloc(sizeof(ei_linked_point_t));
    bottom_left->point.x = frame.top_left.x;
    bottom_left->point.y = frame.top_left.y + frame.size.height;
    last_top_left->next = bottom_left;
    /* bottom right */
    ei_linked_point_t* bottom_right = malloc(sizeof(ei_linked_point_t));
    bottom_right->point.x = frame.top_left.x + frame.size.width;
    bottom_right->point.y = bottom_left->point.y;
    bottom_left->next = bottom_right;
    /* top right */
    center.x = bottom_right->point.x - radius;
    center.y = bottom_left->point.y - radius;
    ei_linked_point_t* first_top_right;
    ei_linked_point_t* last_top_right = arc(&first_top_right, center, radius, 0, M_PI/2);
    bottom_right->next = first_top_right;
    /* last point */
    ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
    assert(last_point != NULL);
    last_point->point.x = first_top_left->point.x;
    last_point->point.y = first_top_left->point.y;
    last_top_right->next = last_point;
    last_point->next = NULL;
    return first_top_left;
}

void draw_toplevel(ei_surface_t surface,
                    ei_rect_t* clipper,
                    ei_size_t bg_size,
                    ei_color_t bg_color,
                    int border_width) {
    int little_border = 3;
    ei_rect_t border_frame = {{0, 0}, {bg_size.width + 2 * little_border, border_width}};
    float factor = 0.3;
    ei_color_t shade = {bg_color.red * (1 - factor),
                        bg_color.green * (1 - factor),
                        bg_color.blue * (1 - factor),
                        bg_color.alpha};
    ei_linked_point_t* pts = toplevel_frame(border_frame);
    ei_draw_polygon(surface, pts, shade, clipper);
    free_linked_point(pts);
    ei_rect_t frame = {{0, border_width}, {bg_size.width + 2 * little_border, bg_size.height + 2 * little_border}};
    ei_fill(surface, &shade, &frame);

    ei_rect_t bg_frame = {{little_border, border_width + little_border},
                          bg_size};
    ei_fill(surface, &bg_color, &bg_frame);
}
