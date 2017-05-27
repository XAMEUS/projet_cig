/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of frame.
 */

#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "ei_widget.h"

typedef struct ei_frame_t {
    ei_widget_t widget;
    ei_relief_t relief;
    ei_color_t bg_color;
    int border_width;
    /* Optionnal text */
    char* text;
    ei_font_t font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    /* Optionnal image */
    ei_surface_t img;
    ei_rect_t* img_rect;
    ei_anchor_t img_anchor;
} ei_frame_t;

#endif
