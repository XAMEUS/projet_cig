#ifndef EI_DRAW_CONTENT_H
#define EI_DRAW_CONTENT_H

void draw_text (struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_rect_t*		clipper,
				int offset);

void draw_image(struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_rect_t*		clipper,
				int offset);

#endif
