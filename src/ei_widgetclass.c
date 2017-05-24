#include "ei_widgetclass.h"
#include <stdlib.h>

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
    return;
}


/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "button" can be created and configured with
 *		\ref ei_button_configure.
 */
void ei_button_register_class() {
    return;
}

/**
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void ei_toplevel_register_class() {
    return;
}
