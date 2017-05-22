#include <assert.h>

#include "ei_draw.h"

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color) {
    int r, g, b, a;
    hw_surface_get_channel_indices(surface, &r, &g, &b, &a);
    uint32_t c = color->red << 8*(3-r) +
                        color->green << 8*(3-g) +
                        color->blue << 8*(3-b) +
                        color->alpha << 8*(3-a);
    return c;
}

void ei_draw_polyline (ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) {

}

typedef struct {
  int y_max;
  int x_ymin;
  float fract;
  polygon_side *next;
} polygon_side;

void extremum_pts(const ei_linked_point_t* first_point,
                  int *xmin,
                  int *xmax,
                  int *ymin,
                  int *ymax;) {
    ei_linked_point_t p = first_point;
    while (p != NULL) {
      if (p.point.x < *xmin) *xmin = p.x;
      else if (p.point.x > *xmax) *max = p.x;
      if (p.point.y < *ymin) *ymin = p.y;
      else if (p.point.y > *ymax) * ymax = p.y;
      p = p.next;
    }
}

void insert_side(polygon_side* head,
                 polygon_side* side) {
  if (head == NULL) {
    head = side;
    side.next = NULL;
  } else if (side.y_max < head.y_max) {
    side.next = head;
    head = side;
  } else {
    polygon_side* curr, next = head, head.next;
    while (next != NULL) {
      if (side.y_max < next.y_max) {
        curr.next = side;
        side.next = next;
        return;
      }
      curr = curr.next;
      next = next.next;
    }
    curr.next = side; // insertion en fin
  }
}

void init_TC(polygon_side* TC[]) {
  ei_linked_point_t *curr = first_point, *next = first_point.next;
  while (next != NULL) {
    polygon_side* side = malloc(sizeof(polygon_side));
    assert(side != NULL);
    side.fract = (next.point.x - curr.point.x) / (next.point.y - curr.point.y);
    if (curr.point.y > next.point.y) {
      side.y_max = curr.point.y;
      side.x_ymin = next.point.x;
      insert_side(TC[next.point.y], side);
    } else {
      side.y_max = next.point.y;
      side.x_ymin = curr.point.x;
      insert_side(TC[curr.point.y], side);
    }
    curr = curr.next;
    next = next.next;
  }
}

void ei_draw_polygon(ei_surface_t surface,
						 const ei_linked_point_t* first_point,
						 const ei_color_t color,
						 const ei_rect_t* clipper) {
    if (first_point != NULL) {
      hw_surface_lock(surface);
      int xmin = length(surface), xmax = 0;
      int ymin = length(surface), ymax = 0;
      extremum_pts(surface, &xmin, &xmax, &ymin, &ymax);
      //const ei_rect_t rect

      /* Initialisation */
      polygon_side* TC[ymax-ymin];
      init_TC(TC);
      polygon_side* TCA = NULL;

      /* Dessin du polygone */
      // TODO


      hw_surface_unlock(surface);
      // hw_surface_update_rects(surface, rect);
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
    return NULL;
}
