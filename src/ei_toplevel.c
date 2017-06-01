#include <stdlib.h>
#include <string.h>
#include "ei_toplevel.h"
#include "ei_draw_ex.h"

static void* ei_toplevel_alloc();
static void ei_toplevel_release_func(struct ei_widget_t* widget);
static void ei_toplevel_drawfunc(struct ei_widget_t*	widget,
    							 ei_surface_t		surface,
    							 ei_surface_t		pick_surface,
    							 ei_rect_t*		clipper);
static void ei_toplevel_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_toplevel_handlefunc(struct ei_widget_t*	widget,
						 			 	struct ei_event_t*	event);
static void	ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);


void ei_toplevel_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "toplevel", 20);
    widget->allocfunc = &ei_toplevel_alloc;
    widget->releasefunc = &ei_toplevel_release_func;
    widget->drawfunc = &ei_toplevel_drawfunc;
    widget->setdefaultsfunc = &ei_toplevel_setdefaultsfunc;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc =  &ei_toplevel_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_toplevel_alloc() {
    return calloc(1, sizeof(ei_toplevel_t));
}

static void ei_toplevel_release_func(struct ei_widget_t* widget) {
    return;
}

static void ei_toplevel_drawfunc(struct ei_widget_t*	widget,
    							 ei_surface_t		surface,
    							 ei_surface_t		pick_surface,
    							 ei_rect_t*		clipper) {
	draw_toplevel(surface, clipper, widget->requested_size,
			((ei_toplevel_t*) widget)->bg_color, ((ei_toplevel_t*) widget)->border_width);
}

static void ei_toplevel_setdefaultsfunc(struct ei_widget_t* widget) {
	widget->requested_size.width = 320;
	widget->requested_size.height = 240;
	((ei_toplevel_t*) widget)->bg_color = ei_default_background_color;
	((ei_toplevel_t*) widget)->border_width = 4;
	((ei_toplevel_t*) widget)->title = "Toplevel";
	((ei_toplevel_t*) widget)->closable = EI_TRUE;
	((ei_toplevel_t*) widget)->resizable = ei_axis_both;
}

static ei_bool_t ei_toplevel_handlefunc(struct ei_widget_t*	widget,
						 			 struct ei_event_t*	event) {
    return EI_FALSE;
}

static void	ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
	widget->screen_location = rect;
}
