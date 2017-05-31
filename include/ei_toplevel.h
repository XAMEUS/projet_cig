/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of toplevel.
 */

#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_widget.h"
#include "ei_button.h"

typedef struct ei_toplevel_t {
    ei_widget_t widget;
    ei_color_t bg_color;
    int border_width;
    char* title;
    ei_bool_t closable;
    ei_button_t* close_button;
    ei_axis_set_t resizable;
    ei_button_t* resize_button;
    ei_size_t* min_size;
} ei_toplevel_t;

#endif
