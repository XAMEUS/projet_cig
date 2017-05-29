#include <stdlib.h>
#include "ei_placer.h"
#include "ei_widget.h"

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
    if (widget->placer_params) { // update
        if (anchor) {
            widget->placer_params->anchor = anchor;
            widget->placer_params->anchor_data = *anchor;
        }
        if (x) {
            widget->placer_params->x = x;
            widget->placer_params->x_data = *x;
        }
        if (y) {
            widget->placer_params->y = y;
            widget->placer_params->y_data = *y;
        }
        if (width) {
            widget->placer_params->w = width;
            widget->placer_params->w_data = *width;
        }
        if (height) {
            widget->placer_params->h = height;
            widget->placer_params->h_data = *height;
        }
        if (rel_x) {
            widget->placer_params->rx = rel_x;
            widget->placer_params->rx_data = *rel_x;
        }
        if (rel_y) {
            widget->placer_params->ry = rel_y;
            widget->placer_params->ry_data = *rel_y;
        }
        if (rel_width) {
            widget->placer_params->rw = rel_width;
            widget->placer_params->rw_data = *rel_width;
        }
        if (rel_height) {
            widget->placer_params->rh = rel_height;
            widget->placer_params->rh_data = *rel_height;
        }
    }
    else { // init
        ei_placer_params_t* params = malloc(sizeof(ei_placer_params_t));
        params->anchor = anchor;
        params->x = x;
        params->y = y;
        params->w = width;
        params->h = height;
        params->rx = rel_x;
        params->ry = rel_y;
        params->rw = rel_width;
        params->rh = rel_height;
        if (anchor)
            params->anchor_data = *anchor;
        else
            params->anchor_data = ei_anc_northwest;
        if (x)
            params->x_data = *x;
        else
            params->x_data = 0;
        if (y)
            params->y_data = *y;
        else
            params->y_data = 0;
        if (width)
            params->w_data = *width;
        else if (!rel_width)
            params->w_data = widget->requested_size.width;
        else
            params->w_data = 0;
        if (height)
            params->h_data = *height;
        else if (!rel_height)
            params->h_data = widget->requested_size.height;
        else
            params->w_data = 0;
        if (rel_x)
            params->rx_data = *rel_x;
        else
            params->rx_data = 0.0;
        if (rel_y)
            params->ry_data = *rel_y;
        else
            params->ry_data = 0.0;
        if (rel_width)
            params->rw_data = *rel_width;
        else
            params->rw_data = 0.0;
        if (rel_height)
            params->rh_data = *rel_height;
        else
            params->rh_data = 0.0;
        widget->placer_params = params;
    }
    ei_placer_run(widget);
}


void ei_placer_run(struct ei_widget_t* widget) {

    ei_placer_params_t* params = widget->placer_params;
    widget->screen_location.top_left.x = widget->placer_params->x_data +
            widget->placer_params->rx_data * widget->parent->content_rect->size.width;
    widget->screen_location.top_left.y = widget->placer_params->y_data +
            widget->placer_params->ry_data * widget->parent->content_rect->size.height;

    if (widget->placer_params->rw)
        widget->screen_location.size.width = widget->placer_params->rw_data * widget->parent->content_rect->size.width;
    else
        widget->screen_location.size.width = widget->placer_params->w_data;
    if (widget->placer_params->rh)
        widget->screen_location.size.height = widget->placer_params->rh_data * widget->parent->content_rect->size.height;
    else
        widget->screen_location.size.height = widget->placer_params->h_data;

    switch (widget->placer_params->anchor_data) {
        case ei_anc_center:
            widget->screen_location.top_left.y -= widget->screen_location.size.height/2;
            widget->screen_location.top_left.x -= widget->screen_location.size.width/2;
            break;
        case ei_anc_west:
            widget->screen_location.top_left.y -= widget->screen_location.size.height/2;
            break;
        case ei_anc_east:
            widget->screen_location.top_left.y -= widget->screen_location.size.height/2;
        case ei_anc_northeast:
            widget->screen_location.top_left.x -= widget->screen_location.size.width;
            break;
        case ei_anc_south:
            widget->screen_location.top_left.x -= widget->screen_location.size.width/2;
        case ei_anc_southwest:
            widget->screen_location.top_left.y -= widget->screen_location.size.height;
            break;
        case ei_anc_southeast:
            widget->screen_location.top_left.x -= widget->screen_location.size.width;
            widget->screen_location.top_left.y -= widget->screen_location.size.height;
            break;
        case ei_anc_north:
            widget->screen_location.top_left.x -= widget->screen_location.size.width/2;
            break;
        default:
            break;
    }
    if (widget->parent) {
        widget->screen_location.top_left.x += widget->parent->screen_location.top_left.x;
        widget->screen_location.top_left.y += widget->parent->screen_location.top_left.y;
    }
}



/**
 * \brief	Tells the placer to remove a widget from the screen and forget about it.
 *		Note: the widget is not destroyed and still exists in memory.
 *
 * @param	widget		The widget to remove from screen.
 */
void ei_placer_forget(struct ei_widget_t* widget) {
    return;
}
