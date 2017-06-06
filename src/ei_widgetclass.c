#include "ei_widgetclass.h"
#include "ei_widgetclass_unregister.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ei_widgetclass_t *Classes = NULL;

void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
    widgetclass->next = Classes;
    Classes = widgetclass;
}

ei_widgetclass_t*  ei_widgetclass_from_name(ei_widgetclass_name_t name) {
    ei_widgetclass_t *candidate = Classes;
    while(candidate != NULL && strncmp(candidate->name, name, 20) != 0)
        candidate = candidate->next;
    assert(candidate != NULL); //Maybe useless?
    return candidate;
}

void ei_widgetclass_unregister() {
    ei_widgetclass_t *ptr_class = Classes;
    ei_widgetclass_t *tmp;
    while(ptr_class) {
        if(strncmp(ptr_class->name, "frame", 20) == 0 ||
           strncmp(ptr_class->name, "toplevel", 20) == 0 ||
           strncmp(ptr_class->name, "button", 20) == 0) {
            tmp = ptr_class;
            ptr_class = ptr_class->next;
            free(tmp);
        }
        else
            ptr_class = ptr_class->next;
    }
}
