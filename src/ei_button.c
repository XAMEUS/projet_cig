#include "ei_button.h"
#include "ei_event.h"
#include "ei_application.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static void* ei_button_alloc();
static void ei_button_release_func(struct ei_widget_t* widget);
static void ei_button_setdefaultsfunc(struct ei_widget_t* widget);
static ei_bool_t ei_button_handlefunc(struct ei_widget_t* widget,
						 struct ei_event_t*	event);
static void ei_button_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);
static void	ei_button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);


void ei_button_register_class() {
    ei_widgetclass_t *widget = malloc(sizeof(ei_widgetclass_t));
    strncpy(widget->name, "button", 20);
    widget->allocfunc = &ei_button_alloc;
    widget->releasefunc = &ei_button_release_func;
    widget->drawfunc = &ei_button_drawfunc;
    widget->setdefaultsfunc = &ei_button_setdefaultsfunc;
    widget->geomnotifyfunc = &ei_button_geomnotifyfunc;
    widget->handlefunc = &ei_button_handlefunc;
    ei_widgetclass_register(widget);
}

static void* ei_button_alloc() {
    return calloc(1, sizeof(ei_button_t));
}

static ei_bool_t ei_button_handlefunc(struct ei_widget_t*	widget,
						 struct ei_event_t*	event) {
	if (event->type == ei_ev_mouse_buttondown) {
		((ei_button_t*) widget)->push = EI_TRUE;
		ei_app_invalidate_rect(&widget->screen_location);
		ei_event_set_active_widget(widget);
		return EI_TRUE;

	}
	if (event->type == ei_ev_mouse_buttonup) {
		((ei_button_t*) widget)->push = EI_FALSE;
		ei_app_invalidate_rect(&widget->screen_location);
		if (((ei_button_t*) widget)->callback &&
			widget == ei_widget_pick(&(event->param.mouse.where))) {
			((ei_button_t*) widget)->callback(widget,
											  event,
											  ((ei_button_t*) widget)->user_param);
		}
		ei_event_set_active_widget(NULL);
		return EI_TRUE;
	}
	return EI_FALSE;
}

static void ei_button_release_func(struct ei_widget_t* widget) {
	if(((ei_button_t*) widget)->frame.opt_type == TEXT)
		free(((ei_button_t*) widget)->frame.opt.txt.text);
	else if (((ei_button_t*) widget)->frame.opt_type == IMAGE)
		free(((ei_button_t*) widget)->frame.opt.img.img_rect);
}

static void ei_button_setdefaultsfunc(struct ei_widget_t* widget) {
    ((ei_button_t*) widget)->frame.bg_color = ei_default_background_color;
	((ei_button_t*) widget)->frame.border_width = k_default_button_border_width;
	((ei_button_t*) widget)->frame.relief = ei_relief_raised;
	((ei_button_t*) widget)->frame.opt_type = NONE;
    ((ei_button_t*) widget)->corner_radius = k_default_button_corner_radius;
	widget->content_rect = &(widget->screen_location);
}


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
				ei_surface_t		pick_surface,
				ei_rect_t*		clipper,
				int offset) {
	ei_rect_t text_clipper = widget->screen_location;
	text_clipper.top_left.x += offset;
	text_clipper.top_left.y += offset;
	if(strncmp(widget->wclass->name, "button", 20) == 0 &&
		((ei_button_t*) widget)->push) {
		text_clipper.top_left.x += 2;
		text_clipper.top_left.y += 2;
	}
	text_clipper.size.width -= 2 * offset;
	text_clipper.size.height -= 2 * offset;
	ei_point_t where = {text_clipper.top_left.x,
						text_clipper.top_left.y};
	ei_size_t text_size;
	hw_text_compute_size(((ei_frame_t*) widget)->opt.txt.text, ((ei_frame_t*) widget)->opt.txt.font,
							&text_size.width, &text_size.height);
	place_content(&where, text_clipper.size, ((ei_frame_t*) widget)->opt.txt.text_anchor, text_size);
	ei_draw_text(surface, &where, ((ei_frame_t*) widget)->opt.txt.text, ((ei_frame_t*) widget)->opt.txt.font,
				&((ei_frame_t*) widget)->opt.txt.text_color, &(text_clipper));
}

void draw_image(struct ei_widget_t*	widget,
				ei_surface_t		surface,
				ei_surface_t		pick_surface,
				ei_rect_t*		clipper,
				int offset) {
	ei_rect_t img_clipper = widget->screen_location;
	img_clipper.top_left.x += offset;
	img_clipper.top_left.y += offset;
	if(strncmp(widget->wclass->name, "button", 20) == 0 &&
		((ei_button_t*) widget)->push) {
		text_clipper.top_left.x += 2;
		text_clipper.top_left.y += 2;
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
	ei_rect_t dst_rect = {where, img_size};
	ei_rect_t src_rect = {((ei_frame_t*) widget)->opt.img.img_rect->top_left, img_size};
	int copy = ei_copy_surface(surface, &dst_rect, ((ei_frame_t*) widget)->opt.img.img, &src_rect, EI_FALSE);
	if (copy) fprintf(stderr, "Error: image copy failed.");
}

static void ei_button_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {
    ei_draw_button(surface, clipper,
        widget->screen_location,
        ((ei_button_t*) widget)->corner_radius,
        ((ei_button_t*) widget)->frame.border_width,
        ((ei_button_t*) widget)->frame.bg_color,
		((ei_button_t*) widget)->frame.relief,
        ((ei_button_t*) widget)->push);
	if(((ei_button_t*) widget)->frame.opt_type == TEXT) {
		int offset = ((ei_button_t*) widget)->frame.border_width + ((ei_button_t*) widget)->corner_radius * (1 - sqrt(2)/2);
		draw_text(widget, surface, pick_surface, clipper, offset);
	} else if (((ei_button_t*) widget)->frame.opt_type == IMAGE) {
		int offset = ((ei_button_t*) widget)->frame.border_width + ((ei_button_t*) widget)->corner_radius * (1 - sqrt(2)/2);
		draw_image(widget, surface, pick_surface, clipper, offset);
	}
	ei_draw_polygon(pick_surface,
					rounded_frame(widget->screen_location,
								 ((ei_button_t*) widget)->corner_radius),
					*(widget->pick_color),
					clipper);
}

static void	ei_button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect) {
	//TODO optim
	ei_app_invalidate_rect(&widget->screen_location);
	ei_app_invalidate_rect(&rect);
	widget->screen_location = rect;
	for (ei_widget_t *child = widget->children_head; child != NULL; child = child->next_sibling)
		ei_placer_run(child);
}
