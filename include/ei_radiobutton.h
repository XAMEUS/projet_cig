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

/**
 * @brief	Configures the attributes of widgets of the class "rbutton".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		The widget to configure.
 * @param	number		The id of the widget.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	text		The text to display in the widget.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the text.
 */
void ei_rbutton_configure(ei_widget_t* widget, ei_color_t *bg_color, size_t* number,
                    char **text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor);

/**
 * @brief	Configures the attributes of widgets of the class "ei_radiobutton_configure".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including borders.
 * @param	color		The background color.
 * @param	text		The text to display in the widget.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the text.
 */
void ei_radiobutton_configure(ei_widget_t* widget,
                             ei_size_t* requested_size,
                             const ei_color_t* color,
                             char** text,
                             ei_font_t* text_font,
                             ei_color_t* text_color,
                             ei_anchor_t* text_anchor);

#endif
