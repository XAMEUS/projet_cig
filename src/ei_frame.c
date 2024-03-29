#include <stdlib.h>
#include <string.h>

#include "ei_tools.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_draw.h"
#include "ei_draw_button.h"
#include "ei_draw_content.h"

#include "ei_application.h"

static void* ei_frame_alloc();
static void ei_frame_release_func(struct ei_widget_t* widget);
static void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void ei_frame_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_frame_handlefunc(struct ei_widget_t* widget,
						 			 struct ei_event_t*	event);
// static void	ei_frame_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

void ei_frame_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "frame", 20);
    widget->allocfunc = &ei_frame_alloc;
    widget->releasefunc = &ei_frame_release_func;
    widget->drawfunc = &ei_frame_drawfunc;
    widget->setdefaultsfunc = &ei_frame_setdefaultsfunc;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = &ei_frame_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_frame_alloc() {
    return calloc(1, sizeof(ei_frame_t));
}

static void ei_frame_release_func(struct ei_widget_t* widget) {
	if(((ei_button_t*) widget)->frame.opt_type == TEXT &&
		((ei_button_t*) widget)->frame.opt.txt.text)
			free(((ei_button_t*) widget)->frame.opt.txt.text);
	else if (((ei_button_t*) widget)->frame.opt_type == IMAGE) {
		if(((ei_button_t*) widget)->frame.opt.img.img)
			hw_surface_free(((ei_button_t*) widget)->frame.opt.img.img);
		if(((ei_button_t*) widget)->frame.opt.img.img_rect)
			free(((ei_button_t*) widget)->frame.opt.img.img_rect);
	}
}

static void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
	// ((ei_frame_t*) widget)->bg_color.alpha = 0xFF;
	ei_draw_button(surface, clipper, widget->screen_location, 0, ((ei_frame_t*) widget)->border_width, ((ei_frame_t*) widget)->bg_color, ((ei_frame_t*) widget)->relief, EI_TRUE);
	if(((ei_button_t*) widget)->frame.opt_type == TEXT) {
		draw_text(widget, surface, clipper, ((ei_button_t*) widget)->frame.border_width);
	} else if (((ei_button_t*) widget)->frame.opt_type == IMAGE) {
		draw_image(widget, surface, clipper, ((ei_button_t*) widget)->frame.border_width);
	}
	ei_rect_t *drawing_wall = ei_rect_intrsct(&widget->screen_location, clipper);
	if(drawing_wall)
		ei_fill(pick_surface, widget->pick_color, drawing_wall);
	free(drawing_wall);
}

static void ei_frame_setdefaultsfunc(struct ei_widget_t* widget) {
	((ei_frame_t*) widget)->bg_color = ei_default_background_color;
	((ei_frame_t*) widget)->border_width = 0;
	((ei_frame_t*) widget)->relief = ei_relief_none;
	((ei_frame_t*) widget)->opt_type = NONE;
	widget->content_rect = &(widget->screen_location);
}

static ei_bool_t ei_frame_handlefunc(struct ei_widget_t*	widget,
						 			 struct ei_event_t*	event) {
	return EI_FALSE;
}
