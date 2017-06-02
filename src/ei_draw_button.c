#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "ei_tools.h"
#include "ei_draw.h"
#include "ei_draw_button.h"


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
    int min_size = (frame.size.height < frame.size.width) ? frame.size.height : frame.size.width;
    if (2 * radius > min_size - 2*border) radius = (min_size - 2*border)/2;

    if (border) { //&& relief != ei_relief_none
        assert(2 * border <= min_size);
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
        frame.size.width -= 2 * border;
        frame.size.height -= 2 * border;
        frame.top_left.x +=  border;
        frame.top_left.y += border;
    }

    if (radius) {
        ei_linked_point_t* all_pts = rounded_frame(frame, radius);
        ei_draw_polygon(surface, all_pts, color, clipper);
        free_linked_point(all_pts);
    } else {
        ei_rect_t *drawing_wall = ei_rect_intrsct(&frame, clipper);
        if(drawing_wall)
            ei_fill(surface, &color, drawing_wall);
        free(drawing_wall);
    }
}
