#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ei_button.h"
#include "ei_draw_button.h"
#include "ei_draw_content.h"
#include "ei_event.h"
#include "ei_application.h"

static void* ei_button_alloc();
static void ei_button_release_func(struct ei_widget_t* widget);
static void ei_button_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_button_handlefunc(struct ei_widget_t* widget,
						 struct ei_event_t*	event);
static void ei_button_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void	ei_button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);


void ei_button_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "button", 20);
    widget->allocfunc = &ei_button_alloc;
    widget->releasefunc = &ei_button_release_func;
    widget->drawfunc = &ei_button_drawfunc;
    widget->setdefaultsfunc = &ei_button_setdefaultsfunc;
    widget->geomnotifyfunc = &ei_button_geomnotifyfunc;
    widget->handlefunc = &ei_button_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_button_alloc() {
    return calloc(1, sizeof(ei_button_t));
}

static ei_bool_t ei_button_handlefunc(struct ei_widget_t*	widget,
						 struct ei_event_t*	event) {
	if(ei_event_get_active_widget()) {
		ei_bool_t on_button = widget == ei_widget_pick(&(event->param.mouse.where));
		if(on_button ^ ((ei_button_t*) widget)->push) {
			((ei_button_t*) widget)->push = !((ei_button_t*) widget)->push;
			ei_app_invalidate_rect(&widget->screen_location);
		}
		if (event->type == ei_ev_mouse_buttonup) {
			((ei_button_t*) widget)->push = EI_FALSE;
			ei_app_invalidate_rect(&widget->screen_location);
			if (((ei_button_t*) widget)->callback && on_button) {
				((ei_button_t*) widget)->callback(widget,
												  event,
												  ((ei_button_t*) widget)->user_param);
			}
			ei_event_set_active_widget(NULL);
			return EI_TRUE;
		}
		return EI_FALSE;
	}
	else if (event->type == ei_ev_mouse_buttondown) {
			((ei_button_t*) widget)->push = EI_TRUE;
			ei_app_invalidate_rect(&widget->screen_location);
			ei_event_set_active_widget(widget);
			return EI_TRUE;
	}
	return EI_FALSE;
}

static void ei_button_release_func(struct ei_widget_t* widget) {
	ei_widgetclass_from_name("frame")->releasefunc(widget);
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
    ei_draw_button(surface, clipper,
        widget->screen_location,
        ((ei_button_t*) widget)->corner_radius,
        ((ei_button_t*) widget)->frame.border_width,
        ((ei_button_t*) widget)->frame.bg_color,
		((ei_button_t*) widget)->frame.relief,
        ((ei_button_t*) widget)->push);
	if(((ei_button_t*) widget)->frame.opt_type == TEXT) {
		int offset = ((ei_button_t*) widget)->frame.border_width + ((ei_button_t*) widget)->corner_radius * (1 - sqrt(2)/2);
		draw_text(widget, surface, clipper, offset);
	} else if (((ei_button_t*) widget)->frame.opt_type == IMAGE) {
		int offset = ((ei_button_t*) widget)->frame.border_width + ((ei_button_t*) widget)->corner_radius * (1 - sqrt(2)/2);
		draw_image(widget, surface, clipper, offset);
	}
	ei_linked_point_t* pts = rounded_frame(widget->screen_location,
				 ((ei_button_t*) widget)->corner_radius);
	ei_draw_polygon(pick_surface,
					pts,
					*(widget->pick_color),
					clipper);
	free_linked_point(pts);
}

static void	ei_button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
	widget->screen_location = rect;
}
