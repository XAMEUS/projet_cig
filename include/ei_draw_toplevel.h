/**
* @file ei_draw_toplevel.h
*
* @brief Graphical primitive to draw toplevel.
*/

#ifndef EI_DRAW_TOPLEVEL_H
#define EI_DRAW_TOPLEVEL_H

#include "ei_types.h"

/**
* @brief Draws a toplevel.
*
* @param surface The surface where to draw the toplevel.
*
* @param clipper If not NULL, the drawing is restricted within this rectangle.
*
* @param frame The rectangle representing the content rectangle of the toplevel.
*
* @param bg_color The color of the content rectangle of the toplevel.
*
* @param border_width The width in pixel of the decorations around the content rectangle of the toplevel. The title bar is automatically added and cannot be configured.
*/
void draw_toplevel(ei_surface_t surface,
                    ei_rect_t* clipper,
                    ei_rect_t frame,
                    ei_color_t bg_color,
                    int border_width);

#endif
