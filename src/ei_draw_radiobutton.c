#include <stdlib.h>
#include <assert.h>

#include "ei_draw_radiobutton.h"
#include "ei_draw.h"
#include "ei_draw_button.h"


ei_linked_point_t* diamond(ei_rect_t frame,
                            ei_bool_t up,
                            ei_bool_t down) {
    ei_linked_point_t* left_point = malloc(sizeof(ei_linked_point_t));
    assert(left_point != NULL);
    left_point->point.x = frame.top_left.x;
    left_point->point.y = frame.top_left.y + frame.size.height/2;
    ei_linked_point_t* top_point;
    if (up) {
        top_point = malloc(sizeof(ei_linked_point_t));
        assert(top_point != NULL);
        top_point->point.x = frame.top_left.x + frame.size.width/2;
        top_point->point.y = frame.top_left.y;
        left_point->next = top_point;
    }
    ei_linked_point_t* right_point = malloc(sizeof(ei_linked_point_t));
    assert(right_point != NULL);
    right_point->point.x = frame.top_left.x + frame.size.width;
    right_point->point.y = left_point->point.y;
    if (down && !up) left_point->next = right_point;
    else top_point->next = right_point;
    ei_linked_point_t* bottom_point;
    if (down) {
        bottom_point = malloc(sizeof(ei_linked_point_t));
        assert(bottom_point != NULL);
        bottom_point->point.x = top_point->point.x;
        bottom_point->point.y = frame.top_left.y + frame.size.height;
        right_point->next = bottom_point;
    }
    ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
    assert(last_point != NULL);
    last_point->point.x = left_point->point.x;
    last_point->point.y = left_point->point.y;
    if (up && !down) right_point->next = last_point;
    else bottom_point->next = last_point;
    last_point->next = NULL;
    return left_point;
}

void ei_draw_radiobutton(ei_surface_t surface,
                    ei_rect_t* clipper,
                    ei_point_t top_left,
                    int size,
                    ei_color_t color,
                    ei_bool_t push) {
    ei_rect_t diamond_frame = {top_left, {size, size}};
    ei_linked_point_t* up_pts = diamond(diamond_frame, EI_TRUE, EI_FALSE);
    ei_linked_point_t* down_pts = diamond(diamond_frame, EI_FALSE, EI_TRUE);
    float factor = 0.1;
    ei_color_t shade = {color.red * (1 - factor), color.green * (1 - factor), color.blue * (1 - factor), color.alpha};
    ei_color_t tint = {color.red + (255 - color.red) * factor, color.green + (255 - color.green) * factor, color.blue + (255 - color.blue) * factor, color.alpha};

    int border = size/5;
    diamond_frame.size.width -= 2 * border;
    diamond_frame.size.height -= 2 * border;
    diamond_frame.top_left.x +=  border;
    diamond_frame.top_left.y += border;
    ei_linked_point_t* diamond_pts = diamond(diamond_frame, EI_TRUE, EI_TRUE);

    if (push) {
        float push_factor = 0.6;
        ei_color_t push_color = {color.red * (1 - push_factor), color.green * (1 - push_factor), color.blue * (1 - push_factor), color.alpha};
        ei_draw_polygon(surface, up_pts, shade, clipper);
        ei_draw_polygon(surface, down_pts, tint, clipper);
        ei_draw_polygon(surface, diamond_pts, push_color, clipper);
    } else {
        ei_draw_polygon(surface, up_pts, tint, clipper);
        ei_draw_polygon(surface, down_pts, shade, clipper);
        ei_draw_polygon(surface, diamond_pts, color, clipper);
    }

   free_linked_point(up_pts);
   free_linked_point(down_pts);
   free_linked_point(diamond_pts);
}
