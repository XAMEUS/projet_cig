/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of button.
 */

#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_widget.h"
#include "ei_frame.h"

typedef struct ei_button_t {
    ei_frame_t frame;
    int corner_radius;
    ei_callback_t callback;
	void* user_param;
} ei_button_t;


#endif
