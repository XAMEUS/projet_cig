/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of frame.
 */

#ifndef EI_FRAME_H
#define EI_FRAME_H
#include "ei_widget.h"

struct ei_text {
    char* text;
    ei_font_t font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
};

struct ei_image {
    ei_surface_t img;
    ei_rect_t* img_rect;
    ei_anchor_t img_anchor;
};

typedef union ei_text_or_img {
    struct ei_text txt;
    struct ei_image img;
} ei_text_or_img;

typedef enum {
	NONE = 0,
	TEXT = 1,
    IMAGE = 2
} ei_opt_type;

typedef struct ei_frame_t {
    ei_widget_t widget;
    ei_relief_t relief;
    ei_color_t bg_color;
    int border_width;
    /* Optionnal text or image*/
    ei_opt_type opt_type;
    ei_text_or_img opt;
} ei_frame_t;

#endif
