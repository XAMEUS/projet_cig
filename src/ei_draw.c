#include <assert.h>
#include <math.h>
#include <stdbool.h>

#include "ei_draw.h"
#include <stdlib.h>
#include <stdio.h>

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

struct polygon_side{
  int y_max;
  int x_ymin;
  int dy;
  int err;
  int incr_x;
  struct polygon_side* next;
};

void print_TC(struct polygon_side* TC[], const int TC_size) {
  fprintf(stderr, "TC\n");
  for (int i = 0; i < TC_size; i++) {
    struct polygon_side* s = TC[i];
    if (s != NULL) fprintf(stderr, "\t %d ", i);
    while(s != NULL) {
      fprintf(stderr, "[%d %d] --> ", s->y_max, s->x_ymin);
      s = s->next;
    }
    if (TC[i] != NULL) fprintf(stderr, "\n");
  }
}

void print_TCA(struct polygon_side* TCA) {
  fprintf(stderr, "TCA --> ");
  for (struct polygon_side* s = TCA; s != NULL; s = s->next) {
    fprintf(stderr, "[%d %d] --> ", s->y_max, s->x_ymin);
  }
  fprintf(stderr, "\n");
}

void extremum_pts(const ei_linked_point_t* first_point,
                  ei_point_t* pt_min,
                  ei_point_t* pt_max) {
    for (ei_linked_point_t* p = (ei_linked_point_t*) first_point; p != NULL; p = p->next) {
      if (p->point.x < pt_min->x) pt_min->x = p->point.x;
      else if (p->point.x > pt_max->x) pt_max->x = p->point.x;
      if (p->point.y < pt_min->y) pt_min->y = p->point.y;
      else if (p->point.y > pt_max->y) pt_max->y = p->point.y;
    }
}

struct polygon_side* insert_side(struct polygon_side* head,
                                 struct polygon_side* side) {
  if (head == NULL) {
    head = side;
    side->next = NULL;
  } else if (side->y_max < head->y_max || (side->y_max == head->y_max && side->x_ymin < head->x_ymin)) {
    side->next = head;
    head = side;
  } else {
    struct polygon_side* prec = head, * curr = head->next;
    while (curr != NULL) {
      if (side->y_max < curr->y_max) {
        prec->next = side;
        side->next = curr;
        break;
      } else {
        while (curr != NULL && side->y_max == curr->y_max) {
          if (side->x_ymin < curr->x_ymin) {
            prec->next = side;
            side->next = curr;
            return head;
          }
          prec = curr;
          curr = curr->next;
        }
          prec->next = side;
          side->next = NULL;
      }
    }
    prec->next = side;
    side->next = NULL;
  }
  return head;
}



void init_polygon_side(struct polygon_side* TC[],
                       const int offset,
                       ei_linked_point_t* curr,
                       ei_linked_point_t* next) {
  // fprintf(stderr, "curr : %d %d next : %d %d offset : %d\n", curr->point.x, curr->point.y, curr->next->point.x, curr->next->point.y, offset);
  struct polygon_side* side = malloc(sizeof(struct polygon_side));
  assert(side != NULL);
  side->next = NULL;
  side->dy = abs(next->point.y - curr->point.y);
  side->err = abs(next->point.x - curr->point.x) - side->dy;
  side->incr_x = (curr->point.x < next->point.x) ? 1 : -1;
  // fprintf(stderr, "branch : curr = %d next = %d\n", curr->point.y, next->point.y);
  if (curr->point.y >= next->point.y) {
    side->y_max = curr->point.y;
    side->x_ymin = next->point.x;
    TC[next->point.y - offset] = insert_side(TC[next->point.y - offset], side);
  } else {
    side->y_max = next->point.y;
    side->x_ymin = curr->point.x;
    TC[curr->point.y - offset] = insert_side(TC[curr->point.y - offset], side);
  }
}

void init_TC(struct polygon_side* TC[],
            const int offset,
            const ei_linked_point_t* first_point) {
  ei_linked_point_t* curr = (ei_linked_point_t*) first_point, * next = first_point->next;
  while (next != NULL) {
    init_polygon_side(TC, offset, curr, next);
    curr = curr->next;
    next = next->next;
  }
}

struct polygon_side* remove_side(struct polygon_side* head,
                                 struct polygon_side* side) {
  // fprintf(stderr, "a supprimer : %d %d\n", side->y_max, side->x_ymin);
  if (head == side) {
    head = head->next;
    print_TCA(head);
    free(side);
  } else {
    for (struct polygon_side* prec = head; prec->next != NULL; prec = prec->next) {
      // fprintf(stderr, "prec : %d %d next : %d %d\n", prec->y_max, prec->x_ymin, prec->next->y_max, prec->next->x_ymin);
      if (prec->next == side) {
        prec->next = side->next;
        free(side);
        break;
      }
    }
  }
  return head;
}

// struct polygon_side* sort_TCA(struct polygon_side* head) {
//   struct polygon_side* prec = head, * curr = head->next;
//   while(curr != NULL) {
//     fprintf(stderr, "\n prec : %d %d curr : %d %d\n", prec->y_max, prec->x_ymin, curr->y_max, curr->x_ymin);
//     print_TCA(head);
//     if (curr->y_max < head->y_max || (curr->y_max == head->y_max && curr->x_ymin < head->x_ymin)) {
//       prec->next = curr->next;
//       curr->next = head;
//       head = curr;
//       curr = prec->next;
//       // fprintf(stderr, "fin insert head = prec : %d %d curr : %d %d\n", prec->y_max, prec->x_ymin, curr->y_max, curr->x_ymin);
//       continue;
//     } else {
//       struct polygon_side* s_prec = head, * s_curr = head->next;
//       bool sub_break = false;
//       while (s_curr != curr) {
//         if (curr->y_max < s_curr->y_max) {
//           prec->next = curr->next;
//           curr->next = s_curr;
//           s_prec->next = curr;
//           break;
//         } else {
//           while (curr->y_max == s_curr->y_max) {
//             if (curr->x_ymin < s_curr->x_ymin) {
//               prec->next = curr->next;
//               curr->next = s_curr;
//               s_prec->next = curr;
//               sub_break = true;
//               break;
//             }
//             s_prec = s_prec->next;
//             s_curr = s_curr->next;
//           }
//         }
//         if (sub_break) break;
//         s_prec = s_curr;
//         s_curr = s_curr->next;
//       }
//     }
//     prec = prec->next;
//     curr = prec->next;
//     print_TCA(head);
//   }
//   return head;
// }

struct polygon_side* sort_TCA(struct polygon_side* head) {
    struct polygon_side* new_head = NULL;
    while(head != NULL) {
      struct polygon_side* s = head;
      head = head->next;
      new_head = insert_side(new_head, s);
    }
    return new_head;
}



void ei_draw_polygon(ei_surface_t surface,
						 const ei_linked_point_t* first_point,
						 const ei_color_t color,
						 const ei_rect_t* clipper) {
    if (first_point != NULL && first_point->next->next != NULL) {

      for (ei_linked_point_t* p = (ei_linked_point_t* ) first_point; p != NULL; p = p->next) {
        printf("point %d %d\n", p->point.x, p->point.y);
      }

      ei_point_t pt_min = {(int)INFINITY, (int)INFINITY};
      ei_point_t pt_max = {0, 0};
      extremum_pts(first_point, &pt_min, &pt_max);

      /* Initialisation */
      uint8_t *buff = hw_surface_get_buffer(surface);
	    int W = hw_surface_get_size(surface).width;
	    uint32_t col = ei_map_rgba(surface, &color);
      const int TC_size = pt_max.y - pt_min.y + 1;
      struct polygon_side* TC[TC_size];
      for(int i = 0; i < TC_size; i++) TC[i] = NULL; // necessaire ?
      init_TC(TC, pt_min.y, first_point);
      print_TC(TC, TC_size);
      struct polygon_side* TCA = NULL;


      /* Dessin du polygone */
      int y_scanline = 0;
      while(y_scanline < TC_size || TCA != NULL) {
        getchar();
        fprintf(stderr, "scanline n°%d\n", y_scanline);
        print_TCA(TCA);
        /* Insertion de TC(y_scanline) dans TCA */
        if (y_scanline < TC_size) {
          while (TC[y_scanline] != NULL) {
            struct polygon_side* s = TC[y_scanline];
            TC[y_scanline] = s->next;
            s->next = TCA;
            TCA = s;
          }
        }
        if (TCA != NULL) {
          /*Suppression de TCA(y_max == y_scanline) */
          for(struct polygon_side* s = TCA; s != NULL; s = s->next) {
            if (s->y_max == (y_scanline - pt_min.y)) TCA = remove_side(TCA, s);
          }
          /* Tri par insertion dans TCA */
        if (TCA != NULL) {
          fprintf(stderr, "TCA avant tri\n");
          print_TCA(TCA);
          TCA = sort_TCA(TCA);
          fprintf(stderr, "TCA apres tri\n");
          print_TCA(TCA);
        }
          /* Remplissage (par paire)*/
          // for(struct polygon_side *s = TCA; s != NULL; s = s->next->next) {
          //   for(int x = s->x_ymin; x < s->x_ymin; x++) {
          //     *((uint32_t*)buff + x + W * y_scanline) = col;
          //   }
          // }
    //       /* Passage à la scanline suivante */
    //       y_scanline++;
    //       /* maj segments dans TCA */
    //       for (struct polygon_side* s = TCA; s!= NULL; s = s->next) {
    //         if (2 * s->err > - s->dy) {
    //           s->err -= s->dy;
    //           s->x_ymin += s->incr_x;
    //         }
    //       }
    //     } else {
    //       /* Passage à la scanline suivante */
    //       y_scanline++;
    //     }
      }
    y_scanline++;
    /* update rect */
    // ei_size_t update_size = {pt_max.x - pt_min.x, pt_max.y - pt_min.y};
		// ei_rect_t rect = {pt_min, update_size};
		// ei_linked_rect_t linked_rect = {rect, NULL};
		// hw_surface_update_rects(surface, &linked_rect);
    }
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
						dst_buff[d_pos + a] = 255;
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
