#include "ei_widgetclass.h"
#include "ei_widgetclass_unregister.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ei_widgetclass_t *Classes = NULL;
/**
 * @brief	Registers a class to the program so that widgets of this class can be created.
 *		This must be done only once per widged class in the application.
 *
 * @param	widgetclass	The structure describing the class.
 */
void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
    widgetclass->next = Classes;
    Classes = widgetclass;
}


/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t*  ei_widgetclass_from_name(ei_widgetclass_name_t name) {
    ei_widgetclass_t *candidate = Classes;
    while(candidate != NULL && strncmp(candidate->name, name, 20) != 0)
        candidate = candidate->next;
    assert(candidate != NULL); //Maybe useless?
    return candidate;
}

void ei_widgetclass_unregister() {
    ei_widgetclass_t *tmp;
    while(Classes) {
        tmp = Classes;
        Classes = Classes->next;
        free(tmp);
    }
}
