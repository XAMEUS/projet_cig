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
 * \brief	Fields of button. It has common fields with frame.
 */
typedef struct ei_rbutton_t {
    char* title;
    ei_font_t *font;
    int font_size;
    ei_bool_t pushed; ///< Is the rbutton pushed?
    ei_raddiobutton_t parent;
} ei_rbutton_t;

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
typedef struct ei_raddiobutton_t {
    ei_frame_t frame;
    ei_rbutton_t *selected; ///< The current selected button.
    ei_rbuttons_list list;
} ei_raddiobutton_t;

void add_rbutton(ei_raddiobutton_t, char* title);

#endif
