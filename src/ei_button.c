#include <stdlib.h>
#include <string.h>
#include "ei_button.h"
#include "ei_event.h"

static void* ei_button_alloc();
static void ei_button_release_func(struct ei_widget_t* widget);
static void ei_button_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_button_handlefunc(struct ei_widget_t* widget,
						 struct ei_event_t*	event);
static void ei_button_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);


void ei_button_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "button", 20);
    widget->allocfunc = &ei_button_alloc;
    widget->releasefunc = &ei_button_release_func;
    widget->drawfunc = &ei_button_drawfunc;
    widget->setdefaultsfunc = &ei_button_setdefaultsfunc;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = &ei_button_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_button_alloc() {
    return calloc(1, sizeof(ei_button_t));
}

static ei_bool_t ei_button_handlefunc(struct ei_widget_t*	widget,
						 struct ei_event_t*	event) {
	if (event->type == ei_ev_mouse_buttondown)
		((ei_button_t*) widget)->push = EI_TRUE;
	else if (event->type == ei_ev_mouse_buttonup) {
		((ei_button_t*) widget)->push = EI_FALSE;
		if (((ei_button_t*) widget)->callback)
			((ei_button_t*) widget)->callback(widget, event, ((ei_button_t*) widget)->user_param);
	}
	else return EI_FALSE;
	return EI_TRUE;
}

static void ei_button_release_func(struct ei_widget_t* widget) {
	if(((ei_button_t*) widget)->frame.opt_type == TEXT)
		free(((ei_button_t*) widget)->frame.opt.txt.text);
	else if (((ei_button_t*) widget)->frame.opt_type == IMAGE)
		free(((ei_button_t*) widget)->frame.opt.img.img_rect);
}

static void ei_button_setdefaultsfunc(struct ei_widget_t* widget) {
    ((ei_button_t*) widget)->frame.bg_color = ei_default_background_color;
	((ei_button_t*) widget)->frame.border_width = k_default_button_border_width;
	((ei_button_t*) widget)->frame.relief = ei_relief_raised;
	((ei_button_t*) widget)->frame.opt_type = NONE;
    ((ei_button_t*) widget)->corner_radius = k_default_button_corner_radius;
	widget->content_rect = &(widget->screen_location);
}

static void ei_button_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
    hw_surface_lock	(surface);
    ei_draw_button(surface, clipper,
        widget->screen_location,
        ((ei_button_t*) widget)->corner_radius,
        ((ei_button_t*) widget)->frame.border_width,
        ((ei_button_t*) widget)->frame.bg_color,
        ((ei_button_t*) widget)->push);
    hw_surface_unlock(surface);
	hw_surface_update_rects(surface, NULL);
	hw_surface_lock(pick_surface);
	ei_draw_polygon(pick_surface,
					rounded_frame(widget->screen_location,
								 ((ei_button_t*) widget)->corner_radius),
					*(widget->pick_color),
					clipper);
	printf("%u %u %u %u\n", widget->pick_color->red,
							widget->pick_color->green,
						widget->pick_color->blue,
					widget->pick_color->alpha);
	hw_surface_unlock(pick_surface);
}
