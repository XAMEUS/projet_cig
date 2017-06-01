#include <stdlib.h>
#include <string.h>

#include "ei_toplevel.h"
#include "ei_draw_toplevel.h"
#include "ei_draw_button.h"

#include <stdio.h>

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
    widget->geomnotifyfunc = &ei_toplevel_geomnotifyfunc;
    widget->handlefunc =  &ei_toplevel_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_toplevel_alloc() {
    return calloc(1, sizeof(ei_toplevel_t));
}

static void ei_toplevel_release_func(struct ei_widget_t* widget) {
}

static void ei_toplevel_drawfunc(struct ei_widget_t*	widget,
    							 ei_surface_t		surface,
    							 ei_surface_t		pick_surface,
    							 ei_rect_t*		clipper) {
	draw_toplevel(surface, clipper, widget->screen_location,
			((ei_toplevel_t*) widget)->bg_color, ((ei_toplevel_t*) widget)->border_width);
    fprintf(stderr, "screen location %d %d %d %d\n", widget->screen_location.top_left.x, widget->screen_location.top_left.y, widget->screen_location.size.width, widget->screen_location.size.height);
    ei_point_t text_where = {widget->screen_location.top_left.x, widget->screen_location.top_left.y};
    if (((ei_toplevel_t*) widget)->close_button) {
        ei_point_t button_location = {widget->screen_location.top_left.x + 10, widget->screen_location.top_left.y + 10};
        ei_place(((ei_toplevel_t*) widget)->close_button, NULL, &(button_location.x), &(button_location.y), NULL, NULL, NULL, NULL, NULL, NULL);
        ((ei_toplevel_t*) widget)->close_button->wclass->drawfunc(((ei_toplevel_t*) widget)->close_button, surface, pick_surface, clipper);
        text_where.x += ((ei_toplevel_t*) widget)->close_button->screen_location.size.width;
    }
    if (((ei_toplevel_t*) widget)->resize_button) {
        ei_point_t button_location = {widget->screen_location.top_left.x + widget->screen_location.size.width - ((ei_toplevel_t*) widget)->resize_button->requested_size.width, widget->screen_location.top_left.y + widget->screen_location.size.height - ((ei_toplevel_t*) widget)->resize_button->requested_size.height};
        ei_place(((ei_toplevel_t*) widget)->resize_button, NULL, &(button_location.x), &(button_location.y), NULL, NULL, NULL, NULL, NULL, NULL);
        ((ei_toplevel_t*) widget)->resize_button->wclass->drawfunc(((ei_toplevel_t*) widget)->resize_button, surface, pick_surface, clipper);
    }
    ei_color_t text_color = {255, 255, 255, 255};
    ei_draw_text(surface, &text_where, ((ei_toplevel_t*) widget)->title, ((ei_toplevel_t*) widget)->title_font, &text_color, clipper);
}

static void ei_toplevel_setdefaultsfunc(struct ei_widget_t* widget) {
	widget->requested_size.width = 320;
	widget->requested_size.height = 240;
	((ei_toplevel_t*) widget)->bg_color = ei_default_background_color;
	((ei_toplevel_t*) widget)->border_width = 4;
	((ei_toplevel_t*) widget)->title = "Toplevel";
	((ei_toplevel_t*) widget)->closable = EI_TRUE;
	((ei_toplevel_t*) widget)->resizable = ei_axis_both;
    ((ei_toplevel_t*) widget)->title_font = hw_text_font_create("misc/font.ttf", ei_style_normal, 18);
    widget->screen_location.size.width += widget->requested_size.width + 2 * ((ei_toplevel_t*) widget)->border_width;
    widget->screen_location.size.height += widget->requested_size.height + 2 * ((ei_toplevel_t*) widget)->border_width + BORDER;
}

static ei_bool_t ei_toplevel_handlefunc(struct ei_widget_t*	widget,
						 			 struct ei_event_t*	event) {
    return EI_FALSE;
}

static void	ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
	widget->screen_location = rect;
}
