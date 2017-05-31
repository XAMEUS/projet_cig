#include <stdlib.h>
#include <string.h>
#include "ei_button.h"

static void* ei_button_alloc();
static void ei_button_release_func(struct ei_widget_t* widget);
static void ei_button_setdefaultsfunc(struct ei_widget_t* widget);
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
    widget->handlefunc = NULL;
    ei_widgetclass_register(widget);
}

static void* ei_button_alloc() {
    return calloc(1, sizeof(ei_button_t));
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
	uint32_t *n_buff = (uint32_t *) hw_surface_get_buffer(pick_surface);
	printf("))))))))))) %u %u\n", n_buff[161142], *(widget->pick_color));
	hw_surface_unlock(pick_surface);
}
