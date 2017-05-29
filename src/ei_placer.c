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
    if (widget->placer_params) { // update
        if (anchor)
            widget->placer_params->anchor_data = *anchor;
        if (x)
            widget->placer_params->x_data = *x;
        if (y)
            widget->placer_params->y_data = *y;
        if (width)
            widget->placer_params->w_data = *width;
        if (height)
            widget->placer_params->h_data = *height;
        if (rel_x)
            widget->placer_params->rx_data = *rel_x;
        if (rel_y)
            widget->placer_params->ry_data = *rel_y;
        if (rel_width)
            widget->placer_params->rw_data = *rel_width;
        if (rel_height)
            widget->placer_params->rh_data = *rel_height;
    }
    else { // init
        ei_placer_params_t* params = calloc(1, sizeof(ei_placer_params_t));
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
        ei_rect_t* screen_location = malloc(sizeof(ei_rect_t));
        widget->screen_location = screen_location;
        widget->content_rect = &screen_location;
    }
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
    return;
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
