/**
 * @file	ei_button.h
 *
 * @brief 	Definition of button.
 */

#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_widget.h"
#include "ei_frame.h"

/**
 * \brief	Fields of button. It has common fields with frame.
 */
typedef struct ei_button_t {
    ei_frame_t frame;
    int corner_radius; ///< Corner radius of button.
    ei_callback_t callback; ///< When we click on button.
	void* user_param; ///< Pointer passed to the callback function when it's called..
    ei_bool_t push; ///< Is the button is pushed?
} ei_button_t;


#endif
