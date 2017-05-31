#include "ei_event.h"
#include <stdlib.h>

static ei_widget_t* ACTIVE_WIDGET = NULL;
static ei_default_handle_func_t DEFAULT_HANDLE_FUNCT = NULL;

void ei_event_set_active_widget(ei_widget_t* widget) {
    ACTIVE_WIDGET = widget;
}

ei_widget_t* ei_event_get_active_widget() {
    return ACTIVE_WIDGET;
}

void ei_event_set_default_handle_func(ei_default_handle_func_t func) {
    DEFAULT_HANDLE_FUNCT = func;
}

ei_default_handle_func_t ei_event_get_default_handle_func() {
    return DEFAULT_HANDLE_FUNCT;
}
