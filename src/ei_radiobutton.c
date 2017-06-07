#include <stdlib.h>
#include <string.h>

#include "ei_tools.h"
#include "ei_radiobutton.h"
#include "ei_draw.h"
#include "ei_draw_radiobutton.h"
#include "ei_draw_content.h"

#include "ei_application.h"

void ei_radiobutton_configure(ei_widget_t* widget,
                            ei_size_t* requested_size,
                            const ei_color_t* color,
                            char** text,
                            ei_font_t* text_font,
                            ei_color_t* text_color,
                            ei_anchor_t* text_anchor) {
}

static void* ei_radiobutton_alloc();
static void ei_radiobutton_release_func(struct ei_widget_t* widget);
static void ei_radiobutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void ei_radiobutton_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_radiobutton_handlefunc(struct ei_widget_t* widget,
						 			 struct ei_event_t*	event);
static void	ei_radiobutton_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

void ei_radiobutton_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "radiobutton", 20);
    widget->allocfunc = &ei_radiobutton_alloc;
    widget->releasefunc = &ei_radiobutton_release_func;
    widget->drawfunc = &ei_radiobutton_drawfunc;
    widget->setdefaultsfunc = &ei_radiobutton_setdefaultsfunc;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = &ei_radiobutton_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_radiobutton_alloc() {
    return calloc(1, sizeof(ei_radiobutton_t));
}

static void ei_radiobutton_release_func(struct ei_widget_t* widget) {
    if(((ei_radiobutton_t*) widget)->title) {
        free(((ei_radiobutton_t*) widget)->title->text);
        free(((ei_radiobutton_t*) widget)->title);
    }
}

static void ei_radiobutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
    ei_widget *child = w->children_head;
    while(child) {
        child->wclass->drawfunc(child);
        child = child->next;
    }
	if(((ei_radiobutton_t*) widget)->title && ((ei_radiobutton_t*) widget)->title->text) {
        ei_draw_text(surface,
        					widget->screen_location->top_level,
        					((ei_radiobutton_t*) widget)->title->text,
        					((ei_radiobutton_t*) widget)->title->font,
        					color,
        					clipper);
    }
	ei_rect_t *drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
	if(drawing_wall)
		ei_fill(pick_surface, widget->pick_color, drawing_wall);
	free(drawing_wall);
}

static void ei_radiobutton_setdefaultsfunc(struct ei_widget_t* widget) {
	((ei_radiobutton_t*) widget)->bg_color = ei_default_background_color;
    w->wclass->releasefunc(w);
    ((ei_radiobutton_t*) widget)->title = NULL;
    ((ei_radiobutton_t*) widget)->buttons = NULL;
    ((ei_radiobutton_t*) widget)->callback = NULL;
	widget->content_rect = &(widget->screen_location);
}

static ei_bool_t ei_radiobutton_handlefunc(struct ei_widget_t*	widget,
						 			 struct ei_event_t*	event) {
	return EI_FALSE;
}

/*
 * RBUTTON
 */

static void* ei_rbutton_alloc();
static void ei_rbutton_release_func(struct ei_widget_t* widget);
static void ei_rbutton_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_rbutton_handlefunc(struct ei_widget_t* widget,
						 struct ei_event_t*	event);
static void ei_rbutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void	ei_rbutton_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);


void ei_rbutton_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "rbutton", 20);
    widget->allocfunc = &ei_rbutton_alloc;
    widget->releasefunc = &ei_rbutton_release_func;
    widget->drawfunc = &ei_rbutton_drawfunc;
    widget->setdefaultsfunc = &ei_rbutton_setdefaultsfunc;
    widget->geomnotifyfunc = &ei_rbutton_geomnotifyfunc;
    widget->handlefunc = &ei_rbutton_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_rbutton_alloc() {
    return calloc(1, sizeof(ei_rbutton_t));
}

static ei_bool_t ei_rbutton_handlefunc(struct ei_widget_t*	widget,
						 struct ei_event_t*	event) {
	if(ei_event_get_active_widget()) {
		ei_bool_t on_rbutton = widget == ei_widget_pick(&(event->param.mouse.where));
		if(on_rbutton ^ ((ei_rbutton_t*) widget)->push) {
			((ei_rbutton_t*) widget)->push = !((ei_rbutton_t*) widget)->push;
			ei_app_invalidate_rect(&widget->screen_location);
		}
		if (event->type == ei_ev_mouse_rbuttonup) {
			((ei_rbutton_t*) widget)->push = EI_FALSE;
			ei_app_invalidate_rect(&widget->screen_location);
			if (((ei_rbutton_t*) widget)->callback && on_rbutton) {
				((ei_rbutton_t*) widget)->callback(widget,
												  event,
												  ((ei_rbutton_t*) widget)->user_param);
			}
			ei_event_set_active_widget(NULL);
			return EI_TRUE;
		}
		return EI_FALSE;
	}
	else if (event->type == ei_ev_mouse_rbuttondown) {
			((ei_rbutton_t*) widget)->push = EI_TRUE;
			ei_app_invalidate_rect(&widget->screen_location);
			ei_event_set_active_widget(widget);
			return EI_TRUE;
	}
	return EI_FALSE;
}

static void ei_rbutton_release_func(struct ei_widget_t* widget) {
	// TODO free stuff
}

static void ei_rbutton_setdefaultsfunc(struct ei_widget_t* widget) {
    ((ei_rbutton_t*) widget)->frame.bg_color = ei_default_background_color;
	((ei_rbutton_t*) widget)->frame.border_width = k_default_rbutton_border_width;
	((ei_rbutton_t*) widget)->frame.relief = ei_relief_raised;
	((ei_rbutton_t*) widget)->frame.opt_type = NONE;
    ((ei_rbutton_t*) widget)->corner_radius = k_default_rbutton_corner_radius;
	widget->content_rect = &(widget->screen_location);
}

static void ei_rbutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
    ei_draw_radiobutton(surface, clipper, ((ei_rbutton_t *) widget)->where, 18, ((ei_rbutton_t *) widget)->bg_color, widget->next_sibling? EI_FALSE:EI_TRUE);
    ei_point_t text_where = {((ei_rbutton_t *) widget)->where.x + 3*18, ((ei_rbutton_t *) widget)->where.y}
    ei_draw_text(surface, &text_where, ((ei_rbutton_t *) widget)->text.text, ((ei_rbutton_t *) widget)->text.font, ((ei_rbutton_t *) widget)->text.text_color, clipper);
}

static void	ei_rbutton_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
	ei_app_invalidate_rect(&widget->screen_location);
	widget->screen_location = rect;
	ei_app_invalidate_rect(&widget->screen_location);
}
