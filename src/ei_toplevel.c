#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ei_toplevel.h"
#include "ei_draw_toplevel.h"
#include "ei_draw_button.h"
#include "ei_event.h"

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
    widget->handlefunc = &ei_toplevel_handlefunc;
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
    ei_rect_t *drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
    if(drawing_wall)
    	ei_fill(pick_surface, widget->pick_color, drawing_wall);
    free(drawing_wall);
    ei_point_t text_where = {widget->screen_location.top_left.x, widget->screen_location.top_left.y + 5};
    if (((ei_toplevel_t*) widget)->close_button) {
        ((ei_toplevel_t*) widget)->close_button->wclass->drawfunc(((ei_toplevel_t*) widget)->close_button, surface, pick_surface, clipper);
        text_where.x += 3 * ((ei_toplevel_t*) widget)->close_button->screen_location.size.width;
    }
    if (((ei_toplevel_t*) widget)->resize_button)
        ((ei_toplevel_t*) widget)->resize_button->wclass->drawfunc(((ei_toplevel_t*) widget)->resize_button, surface, pick_surface, clipper);
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
}

static ei_bool_t ei_toplevel_handlefunc(struct ei_widget_t*	widget,
						 			 struct ei_event_t*	event) {
    static ei_point_t old_mouse_pos;
    if(ei_event_get_active_widget()) {
        if(event->type == ei_ev_mouse_buttonup) {
            ei_event_set_active_widget(NULL);
            return EI_TRUE;
        }
        if(event->type == ei_ev_mouse_move) {
            int new_x = widget->placer_params->x_data + event->param.mouse.where.x - old_mouse_pos.x;
            int new_y = widget->placer_params->y_data + event->param.mouse.where.y - old_mouse_pos.y;
            ei_place(widget, NULL, &new_x, &new_y, NULL, NULL, NULL, NULL, NULL, NULL);
            old_mouse_pos = event->param.mouse.where;
            return EI_TRUE;
        }
    }
    else if(event->type == ei_ev_mouse_buttondown &&
        event->param.mouse.where.x >= widget->screen_location.top_left.x &&
        event->param.mouse.where.y >= widget->screen_location.top_left.y &&
        event->param.mouse.where.x <= widget->screen_location.top_left.x +
                                        widget->screen_location.size.width &&
        event->param.mouse.where.y <= widget->screen_location.top_left.y +
                                        BORDER) {
        old_mouse_pos = event->param.mouse.where;
        ei_event_set_active_widget(widget);
        return EI_TRUE;
    }
    return EI_FALSE;
}

static void	ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
    widget->screen_location = rect;
    if(!widget->content_rect || widget->content_rect == & widget->screen_location)
        widget->content_rect = malloc(sizeof(ei_rect_t));
    widget->content_rect->top_left.x = widget->screen_location.top_left.x + ((ei_toplevel_t*) widget)->border_width;
    widget->content_rect->top_left.y = widget->screen_location.top_left.y + BORDER;
    widget->content_rect->size.width = widget->screen_location.size.width - 2 * ((ei_toplevel_t*) widget)->border_width;
    widget->content_rect->size.height = widget->screen_location.size.height - BORDER - ((ei_toplevel_t*) widget)->border_width;
    if(((ei_toplevel_t*) widget)->close_button) {
        ((ei_toplevel_t*) widget)->close_button->content_rect =
            &((ei_toplevel_t*) widget)->close_button->screen_location;
        ((ei_toplevel_t*) widget)->close_button->screen_location.top_left.x =
            widget->screen_location.top_left.x + 10;
        ((ei_toplevel_t*) widget)->close_button->screen_location.top_left.y =
            widget->screen_location.top_left.y + 12;
        ((ei_toplevel_t*) widget)->close_button->screen_location.size.width = 10;
        ((ei_toplevel_t*) widget)->close_button->screen_location.size.height = 10;
    }
    if (((ei_toplevel_t*) widget)->resize_button) {
        ((ei_toplevel_t*) widget)->resize_button->content_rect =
            &((ei_toplevel_t*) widget)->resize_button->screen_location;
            ((ei_toplevel_t*) widget)->resize_button->screen_location.size.width = 10;
            ((ei_toplevel_t*) widget)->resize_button->screen_location.size.height = 10;
        ((ei_toplevel_t*) widget)->resize_button->screen_location.top_left.x = widget->screen_location.top_left.x + widget->screen_location.size.width - ((ei_toplevel_t*) widget)->resize_button->screen_location.size.width;
        ((ei_toplevel_t*) widget)->resize_button->screen_location.top_left.y = widget->screen_location.top_left.y + widget->screen_location.size.height - ((ei_toplevel_t*) widget)->resize_button->screen_location.size.height;
    }

}
