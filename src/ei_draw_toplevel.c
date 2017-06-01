#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "ei_draw.h"
#include "ei_draw_button.h"
#include "ei_toplevel.h"


ei_linked_point_t* toplevel_frame(ei_rect_t frame) {
    int radius = 10;
    /* top left */
    ei_point_t center = {frame.top_left.x + radius, frame.top_left.y + radius};
    ei_linked_point_t* first_top_left = NULL;
    ei_linked_point_t* last_top_left = arc(&first_top_left, center, radius, M_PI/2, M_PI);
    /* bottom left */
    ei_linked_point_t* bottom_left = malloc(sizeof(ei_linked_point_t));
    assert(bottom_left != NULL);
    bottom_left->point.x = frame.top_left.x;
    bottom_left->point.y = frame.top_left.y + frame.size.height;
    last_top_left->next = bottom_left;
    /* bottom right */
    ei_linked_point_t* bottom_right = malloc(sizeof(ei_linked_point_t));
    assert(bottom_right != NULL);
    bottom_right->point.x = frame.top_left.x + frame.size.width;
    bottom_right->point.y = bottom_left->point.y;
    bottom_left->next = bottom_right;
    /* top right */
    center.x = bottom_right->point.x - radius;
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
                    ei_rect_t frame,
                    ei_color_t bg_color,
                    int border_width) {
    ei_rect_t border_frame = {{frame.top_left.x, frame.top_left.y}, {frame.size.width + 2 * border_width, BORDER}};
    ei_color_t shade = {bg_color.red * 0.65,
                        bg_color.green * 0.65,
                        bg_color.blue * 0.65,
                        bg_color.alpha};
    ei_linked_point_t* pts = toplevel_frame(border_frame);
    ei_draw_polygon(surface, pts, shade, clipper);
    free_linked_point(pts);
    ei_rect_t body_frame = {{frame.top_left.x, frame.top_left.y + BORDER}, {frame.size.width + 2 * border_width, frame.size.height + 2 * border_width}};
    ei_fill(surface, &shade, &body_frame);

    ei_rect_t bg_frame = {{frame.top_left.x + border_width, frame.top_left.y + BORDER + border_width}, frame.size};
    ei_fill(surface, &bg_color, &bg_frame);
}
