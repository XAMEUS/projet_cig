#include <assert.h>
#include <math.h>

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

struct polygon_side{
  int y_max;
  int x_ymin;
  int dy;
  int err;
  int incr_x;
  struct polygon_side* next;
};

void extremum_y(ei_linked_point_t* first_point,
                  int* ymin,
                  int* ymax) {
    ei_linked_point_t *p = first_point;
    while (p != NULL) {
      if (p->point.y < *ymin) *ymin = p->point.y;
      else if (p->point.y > *ymax) *ymax = p->point.y;
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

void sort_list(struct polygon_side* head) {
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
        }
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
    if (first_point != NULL) {
      int ymin = (int)INFINITY, ymax = 0;
      extremum_y(surface, &ymin, &ymax);

      /* Initialisation */
      int TC_size = ymax-ymin;
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
          sort_list(TCA);
          /* Remplissage (par paire)*/
          for(struct polygon_side *s = TCA; s != NULL; s = s->next->next) {

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
  }
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
