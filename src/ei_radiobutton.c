#include <stdlib.h>
#include <string.h>

#include "ei_tools.h"
#include "ei_radiobutton.h"
#include "ei_draw.h"
#include "ei_draw_radiobutton.h"
#include "ei_draw_content.h"

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
static void	ei_radiobutton_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

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

void ei_rbutton_configure(ei_widget_t* widget, ei_color_t *bg_color, size_t* number,
                    char **text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor);

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

void ei_add_configure(ei_widget_t* widget, ei_color_t *bg_color, size_t* number,
                    char **text, ei_font_t* font, ei_color_t* text_color, ei_anchor_t* test_anchor) {
    ei_widget_t *rb = ei_widget_create("rbutton", widget);
    ei_rbutton_configure(widget, bg_color, number, text, font, text_color, test_anchor);
    ((ei_rbutton_t*) widget)->number = ((ei_radiobutton_t*) widget)->nb_buttons;
    ((ei_radiobutton_t*) widget)->nb_buttons += 1;
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
    ei_widget_t *child = widget->children_head;
    while(child) {
        child->wclass->drawfunc(child, surface, pick_surface, clipper);
        child = child->next_sibling;
    }
	if(((ei_radiobutton_t*) widget)->title.text) {
        ei_draw_text(surface,
        					&widget->screen_location.top_left,
        					((ei_radiobutton_t*) widget)->title.text,
        					((ei_radiobutton_t*) widget)->title.font,
        					&((ei_radiobutton_t*) widget)->bg_color,
        					clipper);
    }
	ei_rect_t *drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
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
    if(((ei_radiobutton_t*) widget)->buttons)
        free(((ei_radiobutton_t*) widget)->buttons);
    ((ei_radiobutton_t*) widget)->nb_buttons = 0;
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

}

static void ei_rbutton_release_func(struct ei_widget_t* widget) {
	free(((ei_rbutton_t*) widget)->text.text);
}

static void ei_rbutton_setdefaultsfunc(struct ei_widget_t* widget) {
}

static void ei_rbutton_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
    ei_point_t where = {widget->screen_location.top_left.x, widget->screen_location.top_left.y + ((ei_rbutton_t *) widget)->number * ((ei_rbutton_t *) widget)->offset};
    ei_draw_radiobutton(surface, clipper, where, 18, ((ei_rbutton_t *) widget)->bg_color, widget->next_sibling? EI_FALSE : EI_TRUE);
    where.x += 3*18;
    ei_draw_text(surface, &where, ((ei_rbutton_t *) widget)->text.text, ((ei_rbutton_t *) widget)->text.font, &(((ei_rbutton_t *) widget)->text.text_color), clipper);
}
