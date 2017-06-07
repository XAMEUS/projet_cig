/**
* @file ei_draw_content.h
*
* @brief Places and draws texts and images in buttons and frames.
*/

#ifndef EI_DRAW_CONTENT_H
#define EI_DRAW_CONTENT_H

/**
* @brief Places and draws the text of the widget.
*
* @param widget The widget containing the text to draw and its parameters.
*
* @param surface The surface where to draw the text.
*
* @param clipper If not NULL, the drawing is restricted within this rectangle.
*
* @param offset The width in pixel of the decorations (border and rounded corners).
*/
void draw_text(struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_rect_t*		clipper,
				int offset);

/**
* @brief Places and draws the image of the widget.
*
* @param widget The widget containing the image to draw and its parameters.
*
* @param surface The surface where to draw the imae.
*
* @param clipper If not NULL, the drawing is restricted within this rectangle.
*
* @param offset The width in pixel of the decorations (border and rounded corners).
*/
void draw_image(struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_rect_t*		clipper,
				int offset);

#endif
