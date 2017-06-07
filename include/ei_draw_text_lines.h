/**
 *  @file	ei_draw_text_lines.h
 *  @brief	Graphical primitives extension to draw multilines text.
 *
 */

#ifndef EI_DRAW_TEXT_LINES_H
#define EI_DRAW_TEXT_LINES_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"

/**
 * \brief	Draws text by calling \ref hw_text_create_surface. Mangages '\n' and fills the surface available with automatic new lines.
 *
 * @param	surface 	Where to draw the text. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	where		Coordinates, in the surface, where to anchor the top-left corner of
 *				the rendered text.
 * @param	text		The string of the text. Can't be NULL.
 * @param	font		The font used to render the text. If NULL, the \ref ei_default_font
 *				is used.
 * @param	color		The text color. Can't be NULL. The alpha parameter is not used.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_text_lines(ei_surface_t	surface,
					const ei_point_t* where,
					char* text,
					const ei_font_t font,
					const ei_color_t* color,
					const ei_rect_t* clipper);

#endif
