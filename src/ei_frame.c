#include <stdlib.h>
#include <string.h>
#include "ei_frame.h"

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
    if(((ei_frame_t*) widget)->text) free(((ei_frame_t*) widget)->text);
    if(((ei_frame_t*) widget)->img_rect) free(((ei_frame_t*) widget)->img_rect);
}

static void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
    return;
}

static void ei_frame_setdefaultsfunc(struct ei_widget_t* widget) {
    return;
}
