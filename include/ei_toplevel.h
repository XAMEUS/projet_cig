/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of toplevel.
 */

#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_widget.h"
#include "ei_button.h"

#define BORDER 30

typedef struct ei_toplevel_t {
    ei_widget_t widget;
    ei_color_t bg_color;
    int border_width;
    char* title;
    ei_font_t title_font;
    ei_bool_t closable;
    ei_widget_t* close_button;
    ei_axis_set_t resizable;
    ei_widget_t* resize_button;
    ei_size_t* min_size;
} ei_toplevel_t;

#endif
