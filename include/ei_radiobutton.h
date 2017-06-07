/**
 * @file	ei_radiobutton.h
 *
 * @brief 	Definition of radiobutton.
 */

#ifndef EI_RADIOBUTTON_H
#define EI_RADIOBUTTON_H

#include <stdlib.h>
#include "ei_widget.h"
#include "ei_frame.h"

/**
 * \brief	Registers the "radiobutton" widget class in the program. This must be called only
 *		once before widgets of the class "radiobutton" can be created and configured with
 *		\ref ei_radiobutton_configure.
 */
void			ei_radiobutton_register_class 	();

/**
* \brief Widget rbutton
*/
typedef struct ei_rbutton_t {
    ei_widget_t widget; ///< Common to all types of widget.
    ei_color_t bg_color; ///< The background color (with transparency).
    size_t number;
    int offset;
    struct ei_text text;
} ei_rbutton_t;


/**
 * \brief	Widget radiobutton
 */
typedef struct ei_radiobutton_t {
    ei_widget_t widget; ///< Common to all types of widget.
    ei_color_t bg_color; ///< The background color (with transparency).
    ei_callback_t callback; ///< When we click on button.
    struct ei_text title; ///< Optionnal title
    size_t nb_buttons; ///< Number of radion buttons (~ children).
} ei_radiobutton_t;

#endif
