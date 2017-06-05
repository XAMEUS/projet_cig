
#ifndef EI_DRAW_TEXT_LINES_H
#define EI_DRAW_TEXT_LINES_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"


void ei_draw_text_lines(ei_surface_t	surface,
					const ei_point_t* where,
					char* text,
					const ei_font_t font,
					const ei_color_t* color,
					const ei_rect_t* clipper);

#endif
