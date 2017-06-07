/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of frame.
 */

#ifndef EI_FRAME_H
#define EI_FRAME_H
#include "ei_widget.h"

/**
 * \brief	Optionnal attributes if the frame contains a text.
 */
struct ei_text {
    char* text; ///< The text.
    ei_font_t font; ///< The font.
    ei_color_t text_color; ///< The text color.
    ei_anchor_t text_anchor;///< Where we place the text on the frame?
};

/**
 * \brief	Optionnal attributes if the frame contains a image.
 */
struct ei_image {
    ei_surface_t img;  ///< The content of the image.
    ei_rect_t* img_rect;  ///< We can draw only one part of the image.
    ei_anchor_t img_anchor;  ///< Where we place the image on the frame?
};

/**
 * \brief	Union to save memory: We cannot have TEXT and IMAGE in the same time.
 */
typedef union ei_text_or_img {
    struct ei_text txt; ///< If opt_type is a TEXT, we can use this field.
    struct ei_image img; ///< If opt_type is a IMAGE, we can use this field.
} ei_text_or_img;

/**
 * \brief	Type of optionnal fields for frame. It cannot be TEXT and IMAGE both.
 */
typedef enum {
	NONE = 0, ///< Not configured now.
	TEXT = 1, ///< It's a text!
    IMAGE = 2 ///< It's an image.
} ei_opt_type;

/**
 * \brief	Fields of frame. Used by button too.
 */
typedef struct ei_frame_t {
    ei_widget_t widget; ///< Common to all types of widget.
    ei_relief_t relief; ///< The relief of frame.
    ei_color_t bg_color; ///< The background color (with transparency).
    int border_width; ///< The border width.
    /* Optionnal text (exclusive-)or image*/
    ei_opt_type opt_type; ///< Just an enum defined before
    ei_text_or_img opt; ///< The data of optionnal fields.
} ei_frame_t;

#endif
