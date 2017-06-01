#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "ei_draw.h"
#include "ei_draw_button.h"


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
