/**
* @file ei_draw_radiobutton.h
*
* @brief Graphical primitive to draw radiobuttons.
*/

#include "ei_types.h"
#include "hw_interface.h"

/**
* \brief Creates a linked list of points representing a diamond or only the upper or the bottom part.
*
* @param frame The exact rectangle containing the diamond.
*
* @param up A boolean indicating if the upper part of the diamond should be in the list. Should be EI_TRUE to draw a diamond.
*
* @param up A boolean indicating if the bottom part of the diamond should be in the list. Should be EI_TRUE to draw a diamond.
*
* @return The head of the linked list representing the diamond.
*/
ei_linked_point_t* diamond(ei_rect_t frame,
                            ei_bool_t up,
                            ei_bool_t down);

/**
* \brief Draws a radiobutton.
*
* @param surface The surface where to draw the button.
*
* @param clipper If not NULL, the drawing is restricted within this rectangle.
*
* @param top_left The top left point of the frame containing the radiobutton.
*
* @param size The size in pixel of the radiobutton.
*
* @param color The color of the radiobutton. The colors of the border are automaticaly calculated. If pushed, the radiobutton will be darker.
*
* @param push A boolean indicating if the radiobutton is pushed.
*
*/
void ei_draw_radiobutton(ei_surface_t surface,
                    ei_rect_t* clipper,
                    ei_point_t top_left,
                    int size,
                    ei_color_t color,
                    ei_bool_t push);
