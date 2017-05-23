#include <assert.h>
#include <math.h>

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
	uint32_t *buff = (uint32_t *) hw_surface_get_buffer(surface);
	int W = hw_surface_get_size(surface).width;
	uint32_t col = ei_map_rgba(surface, &color);
	ei_point_t pt_min = first_point->point;
	ei_point_t pt_max = first_point->point;
	ei_linked_point_t *ptr_tmp = (ei_linked_point_t *) first_point;
	while(ptr_tmp != NULL) {
		pt_min.x = (pt_min.x > ptr_tmp->point.x) ? pt_min.x : ptr_tmp->point.x;
		pt_min.y = (pt_min.y > ptr_tmp->point.y) ? pt_min.y : ptr_tmp->point.y;
		pt_max.x = (pt_max.x > ptr_tmp->point.x) ? pt_max.x : ptr_tmp->point.x;
		pt_max.y = (pt_max.y > ptr_tmp->point.y) ? pt_max.y : ptr_tmp->point.y;
		ptr_tmp = ptr_tmp->next;
	}
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
			buff[pt_0.x + W * pt_0.y] = col; //TODO clipper
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
	ei_size_t update_size = {pt_max.x - pt_min.x, pt_max.y - pt_min.y};
	ei_rect_t rect = {pt_min, update_size};
	ei_linked_rect_t linked_rect = {rect, NULL};
	hw_surface_update_rects(surface, &linked_rect);
}

struct polygon_side{
  int y_max;
  int x_ymin;
  int dy;
  int err;
  int incr_x;
  struct polygon_side* next;
};

void extremum_pts(ei_linked_point_t* first_point,
                  ei_point_t* pt_min,
                  ei_point_t* pt_max) {
    ei_linked_point_t *p = first_point;
    while (p != NULL) {
      if (p->point.x < pt_min->x) pt_min->x = p->point.x;
      else if (p->point.x > pt_max->x) pt_max->x = p->point.x;
      if (p->point.y < pt_min->y) pt_min->y = p->point.y;
      else if (p->point.y > pt_max->y) pt_max->y = p->point.y;
      p = p->next;
    }
}

// void insert_side(struct polygon_side* head,
//                  struct polygon_side* side) {
//   if (head == NULL) {
//     head = side;
//     side->next = NULL;
//   } else if (side->y_max < head->y_max) {
//     side->next = head;
//     head = side;
//   } else {
//     struct polygon_side* curr = head, * next = head->next;
//     while (next != NULL) {
//       if (side->y_max < next->y_max) {
//         curr->next = side;
//         side->next = next;
//         return;
//       }
//       curr = curr->next;
//       next = next->next;
//     }
//     curr->next = side; // insertion en fin
//   }
// }

void init_polygon_side(struct polygon_side* TC[],
                       ei_linked_point_t* curr,
                       ei_linked_point_t* next) {
  struct polygon_side* side = malloc(sizeof(struct polygon_side));
  assert(side != NULL);
  side->dy = abs(next->point.y - curr->point.y);
  side->err = abs(next->point.x - curr->point.x) - side->dy;
  side->incr_x = (curr->point.x < next->point.x) ? 1 : -1;
  if (curr->point.y > next->point.y) {
    side->y_max = curr->point.y;
    side->x_ymin = next->point.x;
    // insert_side(TC[next->point.y], side);
    side->next = TC[next->point.y];
    TC[next->point.y] = side;
  } else {
    side->y_max = next->point.y;
    side->x_ymin = curr->point.x;
    // insert_side(TC[curr->point.y], side);
    side->next = TC[curr->point.y];
    TC[curr->point.y] = side;
  }
}

void init_TC(struct polygon_side* TC[],
            const ei_linked_point_t* first_point) {
  ei_linked_point_t* curr = (ei_linked_point_t*) first_point, * next = first_point->next;
  while (next != NULL) {
    init_polygon_side(TC, curr, next);
    curr = curr->next;
  }
  init_polygon_side(TC, (ei_linked_point_t*) first_point, curr); // curr = last point
}

void remove_side(struct polygon_side* head,
                 struct polygon_side* side) {
  if (head == side) {
    head = head->next;
    free(side);
  } else {
    for (struct polygon_side* prec = head; prec->next != NULL; prec = prec->next) {
      if (prec->next == side) {
        prec->next = side->next;
        free(side);
      }
    }
  }
}

void sort_TCA(struct polygon_side* head) {
  struct polygon_side* prec = head, * curr = head->next;
  while(curr != NULL) {
    if (curr->y_max < head->y_max) {
      prec->next = curr->next;
      curr->next = head;
      head = curr;
    } else {
      struct polygon_side* s_prec = head, * s_curr = head->next;
      while (s_curr != curr) {
        if (curr->y_max < s_curr->y_max) {
          prec->next = curr->next;
          curr->next = s_curr;
          s_prec->next = curr;
          break;
        } else {
          while (curr->y_max == s_curr->y_max) {
            if (curr->x_ymin < s_curr->x_ymin) {
              prec->next = curr->next;
              curr->next = s_curr;
              s_prec->next = curr;
              break;
            }
            s_prec = s_curr;
            s_curr = s_curr->next;
          }
        }
        s_prec = s_curr;
        s_curr = s_curr->next;
      }
    }
    prec = curr;
    curr = curr->next;
  }
}

void ei_draw_polygon(ei_surface_t surface,
						 const ei_linked_point_t* first_point,
						 const ei_color_t color,
						 const ei_rect_t* clipper) {
    if (first_point != NULL && first_point->next->next != NULL) {
      ei_point_t pt_min = {(int)INFINITY, (int)INFINITY};
      ei_point_t pt_max = {0, 0};
      extremum_pts(surface, &pt_min, &pt_max);

      /* Initialisation */
      uint8_t *buff = hw_surface_get_buffer(surface);
	    int W = hw_surface_get_size(surface).width;
	    uint32_t col = ei_map_rgba(surface, &color);
      int TC_size = pt_max.y - pt_min.y;
      struct polygon_side* TC[TC_size];
      init_TC(TC, first_point);
      struct polygon_side* TCA = NULL;

      /* Dessin du polygone */
      int y_scanline = 0;
      while(y_scanline < TC_size && TCA != NULL) {

        /* Insertion de TC(y_scanline) dans TCA */
        if (y_scanline < TC_size) {
          while (TC[y_scanline] != NULL) {
            struct polygon_side* s = TC[y_scanline];
            TC[y_scanline] = s->next;
            s->next = NULL;
            // insert_side(TCA, s);
            s->next = TCA;
            TCA = s;
          }
        }

        if (TCA != NULL) {
          /*Suppression de TCA(y_max == y_scanline) */
          for(struct polygon_side* s = TCA; s != NULL; s = s->next) {
            if (s->y_max == y_scanline + TC_size) remove_side(TCA, s);
          }
          /* Tri par insertion dans TCA */
          sort_TCA(TCA);
          /* Remplissage (par paire)*/
          for(struct polygon_side *s = TCA; s != NULL; s = s->next->next) {
            for(int x = s->x_ymin; x < s->x_ymin; x++) {
              *((uint32_t*)buff + x + W * y_scanline) = col;
            }
          }
          /* Passage à la scanline suivante */
          y_scanline++;
          /* maj segments dans TCA */
          for (struct polygon_side* s = TCA; s!= NULL; s = s->next) {
            if (2 * s->err > - s->dy) {
              s->err -= s->dy;
              s->x_ymin += s->incr_x;
            }
          }
        } else {
          /* Passage à la scanline suivante */
          y_scanline++;
        }
    }

    /* update rect */
    ei_size_t update_size = {pt_max.x - pt_min.x, pt_max.y - pt_min.y};
		ei_rect_t rect = {pt_min, update_size};
		ei_linked_rect_t linked_rect = {rect, NULL};
		hw_surface_update_rects(surface, &linked_rect);
  }
}

void ei_draw_text(ei_surface_t	surface,
					const ei_point_t* where,
					const char* text,
					const ei_font_t font,
					const ei_color_t* color,
					const ei_rect_t* clipper) {
	ei_surface_t text_surface = hw_text_create_surface(text, font, color);
	ei_rect_t clipper_text = hw_surface_get_rect(text_surface);
	ei_copy_surface(surface, &clipper_text, text_surface, &clipper_text, EI_TRUE);
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

	printf("src (cx,cy) : %d %d\n", src_rect->top_left.x, src_rect->top_left.y);
	printf("dst (cx,cy) : %d %d\n", dst_rect->top_left.x, dst_rect->top_left.y);

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
	printf("src_size: %d %d\n", src_size.width, src_size.height);
	printf("dst_size: %d %d\n", dst_size.width, dst_size.height);
	printf("src (cx,cy) : %d %d\n", src_clipper_pt.x, src_clipper_pt.y);
	printf("dst (cx,cy) : %d %d\n", dst_clipper_pt.x, dst_clipper_pt.y);
	if(clipper_size.width == src_clipper_size.width &&
			clipper_size.height == src_clipper_size.height) {
		printf("%d %d %d %d\n", clipper_size.width, clipper_size.height, src_clipper_size.width, src_clipper_size.height);
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

		printf("%d %d\n", clipper_size.width, clipper_size.height);
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
						uint8_t d = a < 0;
						uint8_t s_r = src_buff[s_pos + (3-r1-d)];
						uint8_t s_g = src_buff[s_pos + (3-g1-d)];
						uint8_t s_b = src_buff[s_pos + (3-b1-d)];
						uint8_t s_a = src_buff[s_pos + (d ? 3:3-a1)];
						uint8_t d_r = dst_buff[d_pos + (3-r-d)];
						uint8_t d_g = dst_buff[d_pos + (3-g-d)];
						uint8_t d_b = dst_buff[d_pos + (3-b-d)];
						dst_buff[d_pos + (3-r-d)] = ((uint16_t) d_r * (255 - s_a) + (uint16_t) s_r * s_a) / 255;
						dst_buff[d_pos + (3-g-d)] = ((uint16_t) d_g * (255 - s_a) + (uint16_t) s_g * s_a) / 255;
						dst_buff[d_pos + (3-b-d)] = ((uint16_t) d_b * (255 - s_a) + (uint16_t) s_b * s_a) / 255;
						// printf("%u\n", src_buff[i*src_size.width + j + src_offset]);
					}
				}
			}
			else {
				uint32_t *src_buff = (uint32_t *) hw_surface_get_buffer(source);
				uint32_t *dst_buff = (uint32_t *) hw_surface_get_buffer(destination);
				for (int i = 0; i < clipper_size.height; i++) {
					for (int j = 0; j < clipper_size.width; j++) {
						dst_buff[i*dst_size.width + j + dst_offset] =
						src_buff[i*src_size.width + j + src_offset];
						//printf("%u\n", src_buff[i*src_size.width + j + src_offset]);
					}
				}
			}
		}
		return 0;
	}
	else return 1;

}
