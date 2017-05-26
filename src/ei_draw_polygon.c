#include "ei_draw.h"
#include "ei_draw_ex.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct polygon_side{
    int y_max;
    float x_ymin;
    float rslope;
    struct polygon_side* next;
};

/* Affichage debug */
void print_TC(struct polygon_side* TC[], const int TC_size) {
  fprintf(stderr, "TC\n");
  for (int i = 0; i < TC_size; i++) {
    struct polygon_side* s = TC[i];
    if (s != NULL) fprintf(stderr, "\t %d ", i);
    while(s != NULL) {
      fprintf(stderr, "[%d %f %f] --> ", s->y_max, s->x_ymin, s->rslope);
      s = s->next;
    }
    if (TC[i] != NULL) fprintf(stderr, "\n");
  }
}

void print_TCA(struct polygon_side* TCA) {
  fprintf(stderr, "TCA --> ");
  for (struct polygon_side* s = TCA; s != NULL; s = s->next) {
    fprintf(stderr, "[%d %f %f] --> ", s->y_max, s->x_ymin, s->rslope);
  }
  fprintf(stderr, "\n");
}
/* fin Affichage debug */

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

struct polygon_side* insert_in_TC(struct polygon_side* head,
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

struct polygon_side* insert_in_TCA(struct polygon_side* head,
                                   struct polygon_side* side) {
    if (head == NULL) {
          head = side;
          side->next = NULL;
    } else if (side->x_ymin < head->x_ymin) {
          side->next = head;
          head = side;
    } else {
        struct polygon_side* prec = head, * curr = head->next;
        while (curr != NULL) {
            if (side->x_ymin < curr->x_ymin) {
                prec->next = side;
                side->next = curr;
                break;
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
      struct polygon_side* side = malloc(sizeof(struct polygon_side));
      assert(side != NULL);
      side->next = NULL;
      side->rslope = (float)(curr->point.x - next->point.x) / (float)(curr->point.y - next->point.y);
      if (curr->point.y >= next->point.y) {
            side->y_max = curr->point.y;
            side->x_ymin = next->point.x;
            TC[next->point.y - offset] = insert_in_TC(TC[next->point.y - offset], side);
      } else {
            side->y_max = next->point.y;
            side->x_ymin = curr->point.x;
            TC[curr->point.y - offset] = insert_in_TC(TC[curr->point.y - offset], side);
      }
}

void init_TC(struct polygon_side* TC[],
            const int offset,
            const ei_linked_point_t* first_point) {
    ei_linked_point_t* curr = (ei_linked_point_t*) first_point, * next = first_point->next;
    while (next != NULL) {
        if (curr->point.y != next->point.y) init_polygon_side(TC, offset, curr, next);
        curr = curr->next;
        next = next->next;
    }
}

struct polygon_side* remove_side(struct polygon_side* head,
                                 struct polygon_side* side) {
    if (head == side) {
        head = head->next;
        free(side);
        side = NULL;
    } else {
        for (struct polygon_side* prec = head; prec->next != NULL; prec = prec->next) {
            if (prec->next == side) {
                prec->next = side->next;
                free(side);
                side = NULL;
                break;
            }
        }
    }
    return head;
}

struct polygon_side* sort_TCA(struct polygon_side* head) {
    struct polygon_side* new_head = NULL;
    while(head != NULL) {
          struct polygon_side* s = head;
          head = head->next;
          new_head = insert_in_TCA(new_head, s);
    }
    return new_head;
}

void ei_draw_polygon(ei_surface_t surface,
						 const ei_linked_point_t* first_point,
						 const ei_color_t color,
						 const ei_rect_t* clipper) {

    if (first_point != NULL && first_point->next != NULL && first_point->next->next != NULL) {

        ei_point_t pt_min = {(int)INFINITY, (int)INFINITY};
        ei_point_t pt_max = {0, 0};
        extremum_pts(first_point, &pt_min, &pt_max);

        ei_size_t box = {pt_max.x - pt_min.x + 1, pt_max.y - pt_min.y + 1};
    	ei_surface_t n_surface = hw_surface_create(surface, &box, EI_TRUE);
    	uint32_t col = ei_map_rgba(n_surface, &color);
    	hw_surface_lock(n_surface);
    	uint32_t *n_buff = (uint32_t *) hw_surface_get_buffer(n_surface);

        /* Initialisation */
        const int TC_size = pt_max.y - pt_min.y + 1;
        struct polygon_side* TC[TC_size];
        for(int i = 0; i < TC_size; i++) TC[i] = NULL;
        init_TC(TC, pt_min.y, first_point);
        struct polygon_side* TCA = NULL;
        int y_scanline = 0;

        /* Dessin du polygone */
        while(y_scanline < TC_size || TCA != NULL) {
            /* Insertion de TC(y_scanline) dans TCA */
            if (y_scanline < TC_size) {
                while (TC[y_scanline] != NULL) {
                    struct polygon_side* s = TC[y_scanline];
                    TC[y_scanline] = s->next;
                    s->next = TCA;
                    TCA = s;
                }
            }
            /*Suppression de TCA(y_max == y_scanline) */
            struct polygon_side* s = TCA, * snext;
            while (s != NULL) {
                 snext = s->next;
                 if (s->y_max == (y_scanline + pt_min.y)) TCA = remove_side(TCA, s);
                 s = snext;
            }
            /* Tri dans TCA */
            TCA = sort_TCA(TCA);
            /* Remplissage (par paire)*/
            for(struct polygon_side *s = TCA; s != NULL && s->next != NULL; s = s->next->next) {
                for(int x = floor(s->x_ymin); x < s->next->x_ymin; x++) {
                    n_buff[x - pt_min.x + box.width * y_scanline] = col;
                }
            }
            /* Passage à la scanline suivante */
            y_scanline++;
            /* maj segments dans TCA */
            for (struct polygon_side* s = TCA; s!= NULL; s = s->next) {
                s->x_ymin = s->x_ymin + s->rslope;
            }
        }
        ei_point_t pt_src = {0, 0};
        ei_point_t pt_dst = pt_min;
        ei_size_t draw_box = box;
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
        ei_copy_surface(surface, &dst_rect, n_surface, &src_rect, EI_TRUE);
        hw_surface_unlock(n_surface);
        hw_surface_free(n_surface);
    }
}
