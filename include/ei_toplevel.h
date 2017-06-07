/**
 * @file	ei_toplevel.h
 *
 * @brief 	API Definition of toplevel.
 */

#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_widget.h"
#include "ei_button.h"

#define BORDER 30

/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_toplevel_t {
    ei_widget_t widget; ///< The widget structure.
    ei_color_t bg_color;    ///< The color of the background of the content of the widget. Defaults to \ref ei_default_background_color.
    int border_width;   ///< The width in pixel of the border of the widget. Defaults to 4.
    char* title;    ///< The string title diplayed in the title bar. Defaults to "Toplevel".
    ei_font_t title_font;   ///< The font used by the title.
    ei_bool_t closable; ///< show a close button in its title bar.
    ei_widget_t* close_button;  ///< The pointer to the close button widget.
    ei_axis_set_t resizable;    ///< The resizable axis (we can lock rezise for specific axis)
    ei_rect_t resize_button;    /// For resizable widgets, the little rectangle used for resize.
    ei_size_t* min_size;    ///< For resizable widgets, defines the minimum size.
} ei_toplevel_t;

#endif
