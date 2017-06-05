#include "ei_event.h"
#include <stdlib.h>

static ei_widget_t* ACTIVE_WIDGET = NULL;
static ei_default_handle_func_t DEFAULT_HANDLE_FUNCT = NULL;

void ei_event_set_active_widget(ei_widget_t* widget) {
    ACTIVE_WIDGET = widget;
    if((widget) && (widget->parent) && (widget->parent->children_tail != widget)) {
        ei_app_invalidate_rect(&widget->screen_location);
        ei_widget_t* prec = widget->parent->children_head;
        if (prec == widget) {
            widget->parent->children_head = widget->next_sibling;
            widget->parent->children_tail->next_sibling = widget;
            widget->parent->children_tail = widget;
        }
        else {
            while(prec && prec->next_sibling != widget)
                prec = prec->next_sibling;
            if(prec) {
                prec->next_sibling = widget->next_sibling;
                widget->parent->children_tail->next_sibling = widget;
                widget->next_sibling = NULL;
                widget->parent->children_tail = widget;
            }
        }
    }
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
