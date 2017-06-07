/**
* @file ei_draw_button.h
*
* @brief Graphical primitives to draw buttons and frames.
*/

#ifndef EI_DRAW_BUTTON_H
#define EI_DRAW_BUTTON_H

#define M_PI    acos(-1)

/**
* \brief Creates a linked list of points representing an arc.
*
* @param first_point The head of the linked list of points.
*
* @param center The point representing the center of the arc.
*
* @param radius The radius in pixel of the arc.
*
* @param first_angle The angle where the arc starts.
*
* @param last_angle The angle where the arc ends.
*
* @return The tail of the linked list of points representing the arc.
*/
ei_linked_point_t* arc(ei_linked_point_t** first_point,
                       const ei_point_t center,
                       const int radius,
                       const float first_angle,
                       const float last_angle);

/**
* \brief Creates a frame with rounded corners.
*
* @param frame The rectangle representing the frame without rounded corners.
*
* @param radius The radius in pixel of the rounded corners.
*
* @return The linked list of points representing the rounded frame.
*/
ei_linked_point_t* rounded_frame(ei_rect_t frame,
                                int radius);

/**
* \brief Frees the linked list of points.
*
* @param first_point The head of the linked list of points.
*/
void free_linked_point(ei_linked_point_t* first_point);

/**
* \brief Draws a button. Also used to draw a frame (a button without rounded corners).
*
* @param surface The surface where to draw the button.
*
* @param clipper If not NULL, the drawing is restricted within this rectangle.
*
* @param radius The radius in pixel of the rounded corners. Will draw a frame if 0.
*
* @param border The width in pixel of the border decoration of the widget.
*
* @param relief Appearance of the border of the widget.

* @param push A boolean indicating the button is pushed.
*/
void ei_draw_button(ei_surface_t surface,
                  ei_rect_t* clipper,
                  ei_rect_t frame,
                  int radius,
                  int border,
                  ei_color_t color,
                  ei_relief_t relief,
                  ei_bool_t push);


#endif
