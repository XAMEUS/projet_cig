#include <stdlib.h>
#include <string.h>
#include "ei_frame.h"

void ei_frame_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "frame", 20);
    widget->allocfunc = NULL;
    widget->releasefunc = NULL;
    widget->drawfunc = NULL;
    widget->setdefaultsfunc = NULL;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = NULL;
    ei_widgetclass_register(widget);
}

//TODO: add functions linked to the class here (with static!)
