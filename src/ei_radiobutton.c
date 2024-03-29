#include <stdlib.h>
#include <string.h>

#include "ei_tools.h"
#include "ei_event.h"
#include "ei_radiobutton.h"
#include "ei_draw.h"
#include "ei_draw_radiobutton.h"
#include "ei_draw_content.h"
#include "ei_draw_button.h"
#include "ei_application.h"

static void* ei_radiobutton_alloc();
static void ei_radiobutton_release_func(struct ei_widget_t* widget);
static void ei_radiobutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void ei_radiobutton_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_radiobutton_handlefunc(struct ei_widget_t* widget,
						 			 struct ei_event_t*	event);
		
static void* ei_rbutton_alloc();
static void ei_rbutton_release_func(struct ei_widget_t* widget);
static void ei_rbutton_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_rbutton_handlefunc(struct ei_widget_t* widget,
						 struct ei_event_t*	event);
static void ei_rbutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

void ei_radiobutton_configure(ei_widget_t* widget,
                            ei_size_t* requested_size,
                            const ei_color_t* color,
                            char** text,
                            ei_font_t* text_font,
                            ei_color_t* text_color,
                            ei_anchor_t* text_anchor) {
    if(color)
        ((ei_radiobutton_t*) widget)->bg_color = *color;
    if(text) {
        if(((ei_radiobutton_t*) widget)->title.text)
            free(((ei_radiobutton_t*) widget)->title.text);
        ((ei_radiobutton_t*) widget)->title.text = malloc(strlen(*text) + 1);
        strcpy(((ei_radiobutton_t*) widget)->title.text, *text);
    }
    if(text_font)
        ((ei_radiobutton_t*) widget)->title.font = *text_font;
    if(text_color)
        ((ei_radiobutton_t*) widget)->title.text_color = *text_color;
    if(text_anchor)
        ((ei_radiobutton_t*) widget)->title.text_anchor = *text_anchor;

    if(requested_size)
        widget->requested_size = *requested_size;
    if(widget->placer_params)
        ei_placer_run(widget);
}

void ei_add_radiobutton(ei_widget_t* widget, ei_color_t *bg_color,
                    char **text, ei_font_t* font, ei_color_t* text_color, ei_anchor_t* test_anchor) {
    ei_widget_t *rb = ei_widget_create("rbutton", widget);
    ei_rbutton_configure(rb, bg_color, &((ei_radiobutton_t*) widget)->nb_buttons, text, font, text_color, test_anchor);
    ((ei_radiobutton_t*) widget)->nb_buttons += 1;
    ei_anchor_t anchor = ei_anc_southwest;
	int size = 18;
    ei_place(rb, &anchor, NULL, NULL, &size, NULL, NULL, NULL, NULL, NULL);
}

void ei_rbutton_configure(ei_widget_t* widget, ei_color_t *bg_color, size_t* number,
                    char **text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor) {
    if(number)
        ((ei_rbutton_t*) widget)->number = *number;
    if(bg_color)
        ((ei_rbutton_t*) widget)->bg_color = *bg_color;
    if(text) {
        if(((ei_rbutton_t*) widget)->text.text)
            free(((ei_rbutton_t*) widget)->text.text);
            ((ei_rbutton_t*) widget)->text.text = malloc(strlen(*text) + 1);
            strcpy(((ei_rbutton_t*) widget)->text.text, *text);
        }
    if(text_font)
        ((ei_rbutton_t*) widget)->text.font = *text_font;
	if ((((ei_rbutton_t*) widget)->text.text && text_font) || (((ei_rbutton_t*) widget)->text.font && text)) {
		int text_width, text_height;
		hw_text_compute_size(((ei_rbutton_t*) widget)->text.text, ((ei_rbutton_t*) widget)->text.font, &text_width, &text_height);
		((ei_rbutton_t*) widget)->offset = text_height;
	}
    if(text_color)
        ((ei_rbutton_t*) widget)->text.text_color = *text_color;
    if(text_anchor)
        ((ei_rbutton_t*) widget)->text.text_anchor = *text_anchor;
    if(widget->placer_params)
        ei_placer_run(widget);
}

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
        free(((ei_radiobutton_t*) widget)->title.text);
}

static void ei_radiobutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
	ei_rect_t *drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
	if(drawing_wall)
		ei_fill(surface, &((ei_radiobutton_t*) widget)->bg_color, drawing_wall);
	free(drawing_wall);
    ei_widget_t *child = widget->children_head;
    while(child) {
        child->wclass->drawfunc(child, surface, pick_surface, clipper);
        child = child->next_sibling;
    }
	if(((ei_radiobutton_t*) widget)->title.text) {
		ei_color_t black = {0, 0, 0, 255};
        ei_draw_text(surface,
        					&widget->screen_location.top_left,
        					((ei_radiobutton_t*) widget)->title.text,
        					((ei_radiobutton_t*) widget)->title.font,
        					&black,
        					clipper);
    }
	drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
	if(drawing_wall)
		ei_fill(pick_surface, widget->pick_color, drawing_wall);
	free(drawing_wall);
}

static void ei_radiobutton_setdefaultsfunc(struct ei_widget_t* widget) {
	((ei_radiobutton_t*) widget)->bg_color = ei_default_background_color;
    widget->wclass->releasefunc(widget);
    if (((ei_radiobutton_t*) widget)->title.text)
        free(((ei_radiobutton_t*) widget)->title.text);
    ((ei_radiobutton_t*) widget)->title.text = NULL;
    ((ei_radiobutton_t*) widget)->title.font = ei_default_font;
    ((ei_radiobutton_t*) widget)->title.text_color.red = 0;
    ((ei_radiobutton_t*) widget)->title.text_color.green = 0;
    ((ei_radiobutton_t*) widget)->title.text_color.blue = 0;
    ((ei_radiobutton_t*) widget)->title.text_color.alpha = 255;
    ((ei_radiobutton_t*) widget)->title.text_anchor = ei_anc_northwest;
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


void ei_rbutton_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "rbutton", 20);
    widget->allocfunc = &ei_rbutton_alloc;
    widget->releasefunc = &ei_rbutton_release_func;
    widget->drawfunc = &ei_rbutton_drawfunc;
    widget->setdefaultsfunc = &ei_rbutton_setdefaultsfunc;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = &ei_rbutton_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_rbutton_alloc() {
    return calloc(1, sizeof(ei_rbutton_t));
}

static ei_bool_t ei_rbutton_handlefunc(struct ei_widget_t*	widget,
						 struct ei_event_t*	event) {
    if(ei_event_get_active_widget()) {
    	if (event->type == ei_ev_mouse_buttonup) {
    		if (((ei_radiobutton_t*) widget->parent)->callback) {
                int number = ((ei_rbutton_t *) widget)->number;
    		    ((ei_radiobutton_t*) widget->parent)->callback(widget, event,
    									    &number);
            }
            ei_event_set_active_widget(NULL);
            return EI_TRUE;
        }
        return EI_FALSE;
    }
    else if (event->type == ei_ev_mouse_buttondown) {
    	ei_event_set_active_widget(widget);
        ei_app_invalidate_rect(&widget->parent->screen_location);
        return EI_TRUE;
    }
    return EI_FALSE;
}

static void ei_rbutton_release_func(struct ei_widget_t* widget) {
	free(((ei_rbutton_t*) widget)->text.text);
}

static void ei_rbutton_setdefaultsfunc(struct ei_widget_t* widget) {
	((ei_rbutton_t*) widget)->bg_color = ei_default_background_color;
	((ei_rbutton_t*) widget)->text.font = ei_default_font;
	((ei_rbutton_t*) widget)->text.text_color = ei_font_default_color;
	((ei_rbutton_t*) widget)->offset = ei_font_default_size;
}

static void ei_rbutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
	int border = 2;
    ei_point_t where = {widget->screen_location.top_left.x + border,
						widget->screen_location.top_left.y + (((ei_rbutton_t *) widget)->number + 1) * ((ei_rbutton_t *) widget)->offset + border};
    ei_draw_radiobutton(surface, clipper, where, 18, ((ei_rbutton_t *) widget)->bg_color, widget->next_sibling? EI_FALSE : EI_TRUE);
	ei_rect_t diamond_frame = {where, {18, 18}};
	ei_linked_point_t* pick_pts = diamond(diamond_frame, EI_TRUE, EI_TRUE);
	ei_draw_polygon(pick_surface, pick_pts, *(widget->pick_color), clipper);
	free_linked_point(pick_pts);

    where.x += 18 + border * 3;
	where.y -= border;
    ei_draw_text(surface, &where, ((ei_rbutton_t *) widget)->text.text, ((ei_rbutton_t *) widget)->text.font, &(((ei_rbutton_t *) widget)->text.text_color), clipper);
}
