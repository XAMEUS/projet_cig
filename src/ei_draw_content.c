#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_button.h"


void place_content(ei_point_t* where,
					ei_size_t widget_size,
					ei_anchor_t anchor,
					ei_size_t content_size) {
	switch (anchor) {
		case ei_anc_center:
			where->x += widget_size.width/2 - content_size.width/2;
			where->y += widget_size.height/2 - content_size.height/2;
			break;
		case ei_anc_north:
			where->x += widget_size.width/2 - content_size.width/2;
			break;
		case ei_anc_northeast:
			where->x += widget_size.width - content_size.width;
			break;
		case ei_anc_south:
			where->x += widget_size.width/2 - content_size.width/2;
			where->y += widget_size.height - content_size.height;
			break;
		case ei_anc_southeast:
			where->x += widget_size.width - content_size.width;
			where->y += widget_size.height - content_size.height;
			break;
		case ei_anc_southwest:
			where->y += widget_size.height - content_size.height;
			break;
		case ei_anc_east:
			where->x += widget_size.width - content_size.width;
			where->y += widget_size.height/2 - content_size.height/2;
			break;
		case ei_anc_west:
			where->y += widget_size.height/2 - content_size.height/2;
			break;
		default:
			break;
	}
}

void draw_text (struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_rect_t*		clipper,
				int offset) {
	ei_rect_t text_box = widget->screen_location;
	text_box.top_left.x += offset;
	text_box.top_left.y += offset;
	if(strncmp(widget->wclass->name, "button", 20) == 0 &&
		((ei_button_t*) widget)->push) {
		text_box.top_left.x += 2;
		text_box.top_left.y += 2;
	}
	text_box.size.width -= 2 * offset;
	text_box.size.height -= 2 * offset;
	ei_point_t where = {text_box.top_left.x,
						text_box.top_left.y};
	ei_size_t text_size;
	hw_text_compute_size(((ei_frame_t*) widget)->opt.txt.text, ((ei_frame_t*) widget)->opt.txt.font,
							&text_size.width, &text_size.height);
	place_content(&where, text_box.size, ((ei_frame_t*) widget)->opt.txt.text_anchor, text_size);
	ei_draw_text(surface, &where, ((ei_frame_t*) widget)->opt.txt.text, ((ei_frame_t*) widget)->opt.txt.font,
				&((ei_frame_t*) widget)->opt.txt.text_color, clipper);
}

void draw_image(struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_rect_t*		clipper,
				int offset) {
	ei_rect_t img_clipper = widget->screen_location;
	img_clipper.top_left.x += offset;
	img_clipper.top_left.y += offset;
	if(strncmp(widget->wclass->name, "button", 20) == 0 &&
		((ei_button_t*) widget)->push) {
		img_clipper.top_left.x += 2;
		img_clipper.top_left.y += 2;
	}
	img_clipper.size.width -= 2 * offset;
	img_clipper.size.height -= 2 * offset;
	ei_point_t where = {0, 0}; ei_size_t img_size = {0, 0};
	if (((ei_frame_t*) widget)->opt.img.img_rect) {
		img_size.width = ((ei_frame_t*) widget)->opt.img.img_rect->size.width;
		img_size.height = ((ei_frame_t*) widget)->opt.img.img_rect->size.height;
	} else {
		img_size = hw_surface_get_size(((ei_frame_t*) widget)->opt.img.img);
	}
	if (img_size.width > img_clipper.size.width)
	 	img_size.width = img_clipper.size.width;
	 if (img_size.height > img_clipper.size.height)
	 	img_size.height = img_clipper.size.height;
	if (img_size.width != img_clipper.size.width || img_size.height != img_clipper.size.height)
		place_content(&where, img_clipper.size, ((ei_frame_t*) widget)->opt.img.img_anchor, img_size);
	where.x += img_clipper.top_left.x; where.y += img_clipper.top_left.y;
	ei_point_t where_img = ((ei_frame_t*) widget)->opt.img.img_rect->top_left;
	if (clipper) {
		if (where.x < clipper->top_left.x) {
			img_size.width -= clipper->top_left.x - where.x;
			where_img.x += clipper->top_left.x - where.x;
			where.x = clipper->top_left.x;
		}
		if (where.y < clipper->top_left.y) {
			img_size.height -= clipper->top_left.y - where.y;
			where_img.y += clipper->top_left.y - where.y;
			where.y = clipper->top_left.y;
		}
	}
	if (img_size.width > 0 && img_size.height > 0) {
		ei_rect_t dst_rect = {where, img_size};
		ei_rect_t src_rect = {where_img, img_size};
		int copy = ei_copy_surface(surface, &dst_rect, ((ei_frame_t*) widget)->opt.img.img, &src_rect, EI_FALSE);
		if (copy) fprintf(stderr, "Error: image copy failed.\n");
	}
}
