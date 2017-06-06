#include <stdlib.h>
#include "ei_placer.h"
#include "ei_widget.h"
#include "ei_application.h"


void ei_place(struct ei_widget_t* widget,
                ei_anchor_t* anchor,
                int* x,
                int* y,
                int* width,
                int* height,
                float* rel_x,
                float* rel_y,
                float* rel_width,
                float* rel_height) {
    if (!(widget->placer_params)) /* init */
        widget->placer_params = calloc(1, sizeof(ei_placer_params_t));
    /* update */
    if (anchor) {
        widget->placer_params->anchor_data = *anchor;
        widget->placer_params->anchor = &(widget->placer_params->anchor_data);
    }
    if (x) {
        widget->placer_params->x_data = *x;
        widget->placer_params->x = &(widget->placer_params->x_data);
    }
    if (y) {
        widget->placer_params->y_data = *y;
        widget->placer_params->y = &(widget->placer_params->y_data);
    }
    if (width) {
        widget->placer_params->w_data = *width;
        widget->placer_params->w = &(widget->placer_params->w_data);
    }
    if (height) {
        widget->placer_params->h_data = *height;
        widget->placer_params->h = &(widget->placer_params->h_data);
    }
    if (rel_x) {
        widget->placer_params->rx_data = *rel_x;
        widget->placer_params->rx = &(widget->placer_params->rx_data);
    }
    if (rel_y) {
        widget->placer_params->ry_data = *rel_y;
        widget->placer_params->ry = &(widget->placer_params->ry_data);
    }
    if (rel_width) {
        widget->placer_params->rw_data = *rel_width;
        widget->placer_params->rw = &(widget->placer_params->rw_data);
    }
    if (rel_height) {
        widget->placer_params->rh_data = *rel_height;
        widget->placer_params->rh = &(widget->placer_params->rh_data);
    }
    ei_placer_run(widget);
}


void ei_placer_run(struct ei_widget_t* widget) {
    ei_rect_t new_screen_location = widget->screen_location;
    if (!(widget->placer_params->anchor))
        widget->placer_params->anchor_data = ei_anc_northwest;
    if (!(widget->placer_params->x))
        widget->placer_params->x_data = 0;
    if (!(widget->placer_params->y))
        widget->placer_params->y_data = 0;
    if (!(widget->placer_params->w) && !(widget->placer_params->rw))
        widget->placer_params->w_data = widget->requested_size.width;
    else if (!(widget->placer_params->w))
        widget->placer_params->w_data = 0;
    if (!(widget->placer_params->h) && !(widget->placer_params->rh))
        widget->placer_params->h_data = widget->requested_size.height;
    else if (!(widget->placer_params->h))
        widget->placer_params->h_data = 0;
    if (!(widget->placer_params->rx))
        widget->placer_params->rx_data = 0.0;
    if (!(widget->placer_params->ry))
        widget->placer_params->ry_data = 0.0;
    if (!(widget->placer_params->rw))
        widget->placer_params->rw_data = 0.0;
    if (!(widget->placer_params->rh))
        widget->placer_params->rh_data = 0.0;

    new_screen_location.top_left.x = widget->placer_params->x_data +
            widget->placer_params->rx_data * widget->parent->content_rect->size.width;
    new_screen_location.top_left.y = widget->placer_params->y_data +
            widget->placer_params->ry_data * widget->parent->content_rect->size.height;

    if (widget->placer_params->rw)
        new_screen_location.size.width = widget->placer_params->rw_data * widget->parent->content_rect->size.width;
    else
        new_screen_location.size.width = widget->placer_params->w_data;
    if (widget->placer_params->rh)
        new_screen_location.size.height = widget->placer_params->rh_data * widget->parent->content_rect->size.height;
    else
        new_screen_location.size.height = widget->placer_params->h_data;

    switch (widget->placer_params->anchor_data) {
        case ei_anc_center:
            new_screen_location.top_left.y -= new_screen_location.size.height/2;
            new_screen_location.top_left.x -= new_screen_location.size.width/2;
            break;
        case ei_anc_west:
            new_screen_location.top_left.y -= new_screen_location.size.height/2;
            break;
        case ei_anc_east:
            new_screen_location.top_left.y -= new_screen_location.size.height/2;
        case ei_anc_northeast:
            new_screen_location.top_left.x -= new_screen_location.size.width;
            break;
        case ei_anc_south:
            new_screen_location.top_left.x -= new_screen_location.size.width/2;
        case ei_anc_southwest:
            new_screen_location.top_left.y -= new_screen_location.size.height;
            break;
        case ei_anc_southeast:
            new_screen_location.top_left.x -= new_screen_location.size.width;
            new_screen_location.top_left.y -= new_screen_location.size.height;
            break;
        case ei_anc_north:
            new_screen_location.top_left.x -= new_screen_location.size.width/2;
            break;
        default:
            break;
    }
    if (widget->parent) {
        new_screen_location.top_left.x += widget->parent->content_rect->top_left.x;
        new_screen_location.top_left.y += widget->parent->content_rect->top_left.y;
    }
    if(new_screen_location.top_left.x != new_screen_location.top_left.x ||
       new_screen_location.top_left.y != new_screen_location.top_left.y ||
       new_screen_location.size.width == new_screen_location.size.width ||
       new_screen_location.size.height == new_screen_location.size.height) {
           ei_app_invalidate_rect(&widget->screen_location);
           ei_app_invalidate_rect(&new_screen_location);
           widget->wclass->geomnotifyfunc(widget, new_screen_location);
           for (ei_widget_t *child = widget->children_head;
                child != NULL;
                child = child->next_sibling)
                    ei_placer_run(child);
       }
}

void ei_placer_forget(struct ei_widget_t* widget) {
    return;
}
