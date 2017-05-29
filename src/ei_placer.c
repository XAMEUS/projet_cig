#include <stdlib.h>
#include "ei_placer.h"
#include "ei_widget.h"

/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 * 		If the widget was already managed by another geometry manager, then it is first
 *		removed from the previous geometry manager.
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then the requested size of the widget is used, i.e. the minimal size
 *		required to display its content.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width
 *				of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height
 *				of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the master, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the master, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the master (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the master (defaults to 0.0).
 */
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
    if (anchor)
        widget->placer_params->anchor_data = *(widget->placer_params->anchor = anchor);
    if (x)
        widget->placer_params->x_data = *(widget->placer_params->x = x);
    if (y)
        widget->placer_params->y_data = *(widget->placer_params->y = y);
    if (width)
        widget->placer_params->w_data = *(widget->placer_params->w = width);
    if (height)
        widget->placer_params->h_data = *(widget->placer_params->h = height);
    if (rel_x)
        widget->placer_params->rx_data = *(widget->placer_params->rx = rel_x);
    if (rel_y)
        widget->placer_params->ry_data = *(widget->placer_params->ry = rel_y);
    if (rel_width)
        widget->placer_params->rw_data = *(widget->placer_params->rw = rel_width);
    if (rel_height)
        widget->placer_params->rh_data = *(widget->placer_params->rh = rel_height);
    ei_placer_run(widget);
}




/**
 * \brief	Tells the placer to recompute the geometry of a widget.
 *		The widget must have been previsouly placed by a call to \ref ei_place.
 *		Geometry re-computation is necessary for example when the text label of
 *		a widget has changed, and thus the widget "natural" size has changed.
 *
 * @param	widget		The widget which geometry must be re-computed.
 */
void ei_placer_run(struct ei_widget_t* widget) {

    if (!(widget->placer_params->anchor))
        widget->placer_params->anchor_data = ei_anc_northwest;
    if (!(widget->placer_params->x))
        widget->placer_params->x_data = 0;
    if (!(widget->placer_params->y))
        widget->placer_params->y_data = 0;
    if (!(widget->placer_params->w) && widget->placer_params->rw)
        widget->placer_params->w_data = widget->requested_size.width;
    else if (!(widget->placer_params->w))
        widget->placer_params->w_data = 0;
    if (!(widget->placer_params->h) && widget->placer_params->rh)
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
