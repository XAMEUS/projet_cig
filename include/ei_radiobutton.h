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
void			ei_rbutton_register_class 	();

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


/**
 * \brief	Adds a new radiobutton.
 *
 * @param	widget     the rbutton pack.
 * @param	bg_color     the background color.
 * @param	number     the text_size.
 * @param	font     the font to use.
 * @param	text     the text to print.
 * @param	text_color     the text color.
 * @param	test_anchor     the test anchor.
 */
void ei_add_radiobutton(ei_widget_t* widget, ei_color_t *bg_color,
                    char **text, ei_font_t* font, ei_color_t* text_color, ei_anchor_t* test_anchor);

void ei_rbutton_configure(ei_widget_t* widget, ei_color_t *bg_color, size_t* number,
                    char **text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor);

void ei_radiobutton_configure(ei_widget_t* widget,
                             ei_size_t* requested_size,
                             const ei_color_t* color,
                             char** text,
                             ei_font_t* text_font,
                             ei_color_t* text_color,
                             ei_anchor_t* text_anchor);

#endif
