#include <stdlib.h>
#include <string.h>
#include "ei_toplevel.h"

void ei_toplevel_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "toplevel", 20);
    widget->allocfunc = NULL;
    widget->releasefunc = NULL;
    widget->drawfunc = NULL;
    widget->setdefaultsfunc = NULL;
    widget->geomnotifyfunc = NULL;
    widget->handlefunc = NULL;
    ei_widgetclass_register(widget);
}

//TODO: add functions linked to the class here (with static!)
