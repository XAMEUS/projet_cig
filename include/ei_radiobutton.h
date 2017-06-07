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
* \brief LinkedList of raddiobuttons
*/
typedef struct ei_rbuttons_list {
    ei_rect_t rect;
    struct ei_rbuttons_list *next;
} ei_rbuttons_list;

/**
 * \brief	Raddiobuttons pack.
 */
typedef struct ei_radiobutton_t {
    ei_frame_t frame;
    struct ei_rbutton_t *selected; ///< The current selected button.
    ei_rbuttons_list list;
} ei_radiobutton_t;

/**
 * \brief	Fields of button. It has common fields with frame.
 */
typedef struct ei_rbutton_t {
    char* title;
    ei_font_t *font;
    int font_size;
    ei_bool_t pushed; ///< Is the rbutton pushed?
    ei_radiobutton_t parent;
} ei_rbutton_t;

void add_rbutton(ei_radiobutton_t, char* title);

#endif
