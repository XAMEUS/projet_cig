/**
 * @file	ei_radiobutton.h
 *
 * @brief 	Definition of button.
 */

#ifndef EI_RADIOBUTTON_H
#define EI_RADIOBUTTON_H

#include "ei_widget.h"
#include "ei_frame.h"

/**
 * \brief	Registers the "radiobutton" widget class in the program. This must be called only
 *		once before widgets of the class "radiobutton" can be created and configured with
 *		\ref ei_radiobutton_configure.
 */
void			ei_radiobutton_register_class 	();

struct ei_rbutton_t;

/**
* \brief Widget rbutton
*/
typedef struct ei_rbutton {
    ei_widget_t widget; ///< Common to all types of widget.
    ei_color_t bg_color; ///< The background color (with transparency).
    ei_point_t where;
    struct ei_text text;
} ei_rbutton;


/**
 * \brief	Widget radiobutton
 */
typedef struct ei_rbutton_t {
    ei_widget_t widget; ///< Common to all types of widget.
    ei_color_t bg_color; ///< The background color (with transparency).
    ei_callback_t callback; ///< When we click on button.
    struct ei_text title; ///< Optionnal title
    ei_rbuttons_list buttons;
} ei_frame_t;

void add_rbutton(ei_raddiobutton_t, char* title);

#endif
