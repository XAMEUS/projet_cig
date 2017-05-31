#include <stdlib.h>
#include <string.h>
#include "ei_frame.h"
#include "ei_draw_ex.h"

static void* ei_frame_alloc();
static void ei_frame_release_func(struct ei_widget_t* widget);
static void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void ei_frame_setdefaultsfunc(struct ei_widget_t* widget);

void ei_frame_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "frame", 20);
    widget->allocfunc = &ei_frame_alloc;
    widget->releasefunc = &ei_frame_release_func;
    widget->drawfunc = &ei_frame_drawfunc;
    widget->setdefaultsfunc = &ei_frame_setdefaultsfunc;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = NULL;
    ei_widgetclass_register(widget);
}

//TODO: add functions linked to the class here (with static!)

static void* ei_frame_alloc() {
    return calloc(1, sizeof(ei_frame_t));
}

static void ei_frame_release_func(struct ei_widget_t* widget) {
	if(((ei_frame_t*) widget)->opt_type == TEXT)
		free(((ei_frame_t*) widget)->opt.txt.text);
	else if (((ei_frame_t*) widget)->opt_type == IMAGE)
		free(((ei_frame_t*) widget)->opt.img.img_rect);
}

static void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
	hw_surface_lock	(surface);
	ei_draw_button(surface, clipper, widget->screen_location, 0, ((ei_frame_t*) widget)->border_width,
						((ei_frame_t*) widget)->bg_color, ((ei_frame_t*) widget)->relief, EI_FALSE);
	hw_surface_unlock(surface);
	hw_surface_update_rects(surface, NULL);

}

static void ei_frame_setdefaultsfunc(struct ei_widget_t* widget) {
	((ei_frame_t*) widget)->bg_color = ei_default_background_color;
	((ei_frame_t*) widget)->border_width = 0;
	((ei_frame_t*) widget)->relief = ei_relief_none;
	((ei_frame_t*) widget)->opt_type = NONE;
	widget->content_rect = &(widget->screen_location);
}
