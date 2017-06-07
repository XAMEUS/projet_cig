#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ei_toplevel.h"
#include "ei_draw_toplevel.h"
#include "ei_draw_button.h"
#include "ei_event.h"
#include "ei_tools.h"
#include "ei_application.h"
#include "ei_picking.h"

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
static void ei_close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param);


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
    if (((ei_toplevel_t*) widget)->title)
        free(((ei_toplevel_t*) widget)->title);
    if (((ei_toplevel_t*) widget)->close_button) {
        ei_widgetclass_from_name("button")->releasefunc(((ei_toplevel_t*) widget)->close_button);
        ((ei_toplevel_t*) widget)->close_button->parent = NULL;
        ei_widget_destroy(((ei_toplevel_t*) widget)->close_button);
    }
    if (((ei_toplevel_t*) widget)->min_size)
        free(((ei_toplevel_t*) widget)->min_size);
    if(((ei_toplevel_t*) widget)->title_font)
        hw_text_font_free(((ei_toplevel_t*) widget)->title_font);
}

static void ei_toplevel_drawfunc(struct ei_widget_t*	widget,
    							 ei_surface_t		surface,
    							 ei_surface_t		pick_surface,
    							 ei_rect_t*		clipper) {
	draw_toplevel(surface, clipper, widget->screen_location,
			((ei_toplevel_t*) widget)->bg_color, ((ei_toplevel_t*) widget)->border_width);
    ei_rect_t *drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
    ei_rect_t n_clipper = *clipper;
    if(drawing_wall) {
    	ei_fill(pick_surface, widget->pick_color, drawing_wall);
        n_clipper = *drawing_wall;
    }
    free(drawing_wall);
    ei_point_t text_where = {widget->screen_location.top_left.x, widget->screen_location.top_left.y + 5};
    if (((ei_toplevel_t*) widget)->close_button) {
        ((ei_toplevel_t*) widget)->close_button->wclass->drawfunc(((ei_toplevel_t*) widget)->close_button, surface, pick_surface, &n_clipper);
        text_where.x += 3 * ((ei_toplevel_t*) widget)->close_button->screen_location.size.width;
    }
    if (((ei_toplevel_t*) widget)->resizable) {
        ei_color_t button_color = {((ei_toplevel_t*) widget)->bg_color.red * 0.65, ((ei_toplevel_t*) widget)->bg_color.green * 0.65, ((ei_toplevel_t*) widget)->bg_color.blue * 0.65, 255};
        ei_rect_t* button_clipper = ei_rect_intrsct(&((ei_toplevel_t*) widget)->resize_button, clipper);
        if (button_clipper) {
            ei_fill(surface, &button_color, button_clipper);
            free(button_clipper);
        }
    }
    ei_color_t text_color = {255, 255, 255, 255};
    ei_draw_text(surface, &text_where, ((ei_toplevel_t*) widget)->title, ((ei_toplevel_t*) widget)->title_font, &text_color, &n_clipper);
}

static void ei_toplevel_setdefaultsfunc(struct ei_widget_t* widget) {
	widget->requested_size.width = 320;
	widget->requested_size.height = 240;
	((ei_toplevel_t*) widget)->bg_color = ei_default_background_color;
	((ei_toplevel_t*) widget)->border_width = 4;
	((ei_toplevel_t*) widget)->title = "Toplevel"; //TODO
	((ei_toplevel_t*) widget)->closable = EI_TRUE;
	((ei_toplevel_t*) widget)->resizable = ei_axis_both;
    ((ei_toplevel_t*) widget)->title_font = hw_text_font_create("misc/font.ttf", ei_style_normal, 18);

    if(!((ei_toplevel_t*) widget)->close_button) {
        ei_widgetclass_t* class = ei_widgetclass_from_name("button");
        ((ei_toplevel_t*) widget)->close_button = class->allocfunc();
        ((ei_toplevel_t*) widget)->close_button->wclass = class;
        ((ei_toplevel_t*) widget)->close_button->parent = widget;
        ((ei_toplevel_t*) widget)->close_button->wclass->setdefaultsfunc(((ei_toplevel_t*) widget)->close_button);
        add_picker(ei_app_picking_list(), ((ei_toplevel_t*) widget)->close_button);
        ei_size_t button_size = {10, 10};
        ei_color_t button_color = {255, 0, 0, 255};
        int button_border = 2;
        int button_radius = 5;
        ei_callback_t	button_callback 	= ei_close_toplevel;
        ei_button_configure(((ei_toplevel_t*) widget)->close_button, &button_size, &button_color, &button_border, &button_radius, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &button_callback, NULL);
    }
}

static ei_bool_t ei_toplevel_handlefunc(struct ei_widget_t*	widget,
						 			 struct ei_event_t*	event) {
    static ei_point_t old_mouse_pos;
    static enum {NONE, MOVE, RESIZE} type;
    if(ei_event_get_active_widget()) {
        if(event->type == ei_ev_mouse_buttonup) {
            ei_event_set_active_widget(NULL);
            return EI_TRUE;
        }
        if(event->type == ei_ev_mouse_move) {
            if(type == MOVE) {
                int new_x = widget->placer_params->x_data + event->param.mouse.where.x - old_mouse_pos.x;
                int new_y = widget->placer_params->y_data + event->param.mouse.where.y - old_mouse_pos.y;
                ei_place(widget, NULL, &new_x, &new_y, NULL, NULL, NULL, NULL, NULL, NULL);
                old_mouse_pos = event->param.mouse.where;
                return EI_TRUE;
            }
            if(type == RESIZE) {
                int new_x = widget->placer_params->w_data;
                int new_y = widget->placer_params->h_data;
                if (((ei_toplevel_t*) widget)->resizable != ei_axis_y) {
                    new_x += event->param.mouse.where.x - old_mouse_pos.x;
                    new_x = (new_x >= 50) ? new_x : 50;
                }
                if (((ei_toplevel_t*) widget)->resizable != ei_axis_x) {
                    new_y += event->param.mouse.where.y - old_mouse_pos.y;
                    new_y = (new_y >= BORDER * 2) ? new_y : BORDER * 2;
                }
                ei_place(widget, NULL, NULL, NULL, &new_x, &new_y, NULL, NULL, NULL, NULL);
                old_mouse_pos = event->param.mouse.where;
                return EI_TRUE;

            }
        }
    }
    else if(event->type == ei_ev_mouse_buttondown) {
        if(event->param.mouse.where.x >= widget->screen_location.top_left.x &&
            event->param.mouse.where.y >= widget->screen_location.top_left.y &&
            event->param.mouse.where.x <= widget->screen_location.top_left.x +
                                        widget->screen_location.size.width &&
            event->param.mouse.where.y <= widget->screen_location.top_left.y +
                                        BORDER) {
            old_mouse_pos = event->param.mouse.where;
            type = MOVE;
            ei_event_set_active_widget(widget);
            return EI_TRUE;
        }
        if(event->param.mouse.where.x >= ((ei_toplevel_t*) widget)->resize_button.top_left.x &&
            event->param.mouse.where.y >= ((ei_toplevel_t*) widget)->resize_button.top_left.y &&
            event->param.mouse.where.x <= ((ei_toplevel_t*) widget)->resize_button.top_left.x +
                                        ((ei_toplevel_t*) widget)->resize_button.size.width &&
            event->param.mouse.where.y <= ((ei_toplevel_t*) widget)->resize_button.top_left.y +
                                        ((ei_toplevel_t*) widget)->resize_button.size.height) {
            old_mouse_pos = event->param.mouse.where;
            type = RESIZE;
            ei_event_set_active_widget(widget);
            return EI_TRUE;
        }
    }
    return EI_FALSE;
}

static void	ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
    if(!widget->content_rect || widget->content_rect == & widget->screen_location)
        widget->content_rect = malloc(sizeof(ei_rect_t));
    widget->content_rect->top_left.x = rect.top_left.x + ((ei_toplevel_t*) widget)->border_width;
    widget->content_rect->top_left.y = rect.top_left.y + BORDER + ((ei_toplevel_t*) widget)->border_width;
    widget->content_rect->size.width = rect.size.width - 2 * ((ei_toplevel_t*) widget)->border_width;
    widget->content_rect->size.height = rect.size.height - BORDER - 2 * ((ei_toplevel_t*) widget)->border_width;
    if(((ei_toplevel_t*) widget)->min_size) {
        if(((ei_toplevel_t*) widget)->min_size->width > widget->placer_params->w_data)
            ei_place(widget, NULL, NULL, NULL, &((ei_toplevel_t*) widget)->min_size->width, NULL, NULL, NULL, NULL, NULL);
        if(((ei_toplevel_t*) widget)->min_size->height > widget->placer_params->h_data)
            ei_place(widget, NULL, NULL, NULL, NULL, &((ei_toplevel_t*) widget)->min_size->height, NULL, NULL, NULL, NULL);
    }
    if(((ei_toplevel_t*) widget)->close_button) {
        ((ei_toplevel_t*) widget)->close_button->content_rect =
            &((ei_toplevel_t*) widget)->close_button->screen_location;
        ((ei_toplevel_t*) widget)->close_button->screen_location.top_left.x =
            rect.top_left.x + 10;
        ((ei_toplevel_t*) widget)->close_button->screen_location.top_left.y =
            rect.top_left.y + 12;
        ((ei_toplevel_t*) widget)->close_button->screen_location.size.width = 10;
        ((ei_toplevel_t*) widget)->close_button->screen_location.size.height = 10;
    }
    if (((ei_toplevel_t*) widget)->resizable) {
        ((ei_toplevel_t*) widget)->resize_button.top_left.x = rect.top_left.x + rect.size.width - 10;
        ((ei_toplevel_t*) widget)->resize_button.top_left.y = rect.top_left.y + rect.size.height - 10;
        ((ei_toplevel_t*) widget)->resize_button.size.width = ((ei_toplevel_t*) widget)->resize_button.size.height = 10;
    }
}

static void ei_close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	ei_widget_destroy(widget->parent);
}
