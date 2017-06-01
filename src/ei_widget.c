#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_picking.h"
#include "ei_application.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

ei_widget_t* ei_widget_create(ei_widgetclass_name_t	class_name, ei_widget_t* parent) {
    assert(parent);
    ei_widgetclass_t* class = ei_widgetclass_from_name(class_name);
    ei_widget_t *widget = class->allocfunc();
    widget->wclass = class;
    widget->parent = parent;
    if(parent->children_head == NULL)
        parent->children_head = parent->children_tail = widget;
    else
        parent->children_tail = parent->children_tail->next_sibling = widget;
    widget->wclass->setdefaultsfunc(widget);
    add_picker(ei_app_picking_list(), widget);
    return widget;
}

void ei_widget_destroy (ei_widget_t* widget) {
    ei_widget_t *to_free = widget;
    ei_widget_t *tmp;
    while(to_free) {
        while(to_free->children_head)
            to_free = to_free->children_head;

        if(to_free->parent) {
            if(to_free->parent->children_head == to_free)
                to_free->parent->children_head = to_free->next_sibling;
            if(to_free->parent->children_tail == to_free)
                to_free->parent->children_tail = NULL;
        }

        tmp = to_free;
        if(tmp->placer_params)
            free(tmp->placer_params);
        if(tmp->content_rect && tmp->content_rect != &tmp->screen_location)
            free(tmp->content_rect);
        if(to_free->next_sibling)
            to_free = to_free->next_sibling;
        else if(to_free != widget)
            to_free = to_free->parent;
        else
            to_free = NULL;
        tmp->wclass->releasefunc(tmp);
        free(tmp);
    }
}

ei_widget_t* ei_widget_pick(ei_point_t* where) {
    ei_surface_t pick_surface = ei_app_picking_object();
    ei_size_t pick_size = hw_surface_get_size(pick_surface);
    uint32_t number = pick_size.width * where->y + where->x;
    hw_surface_lock(pick_surface);
    uint8_t *n_buff = hw_surface_get_buffer(pick_surface);
    int r, g, b, a;
    hw_surface_get_channel_indices(pick_surface, &r, &g, &b, &a);
    n_buff += number * 4;
    ei_color_t color = {n_buff[r], n_buff[b], n_buff[g], 0x00};
    ei_widget_t * result = take_picker(ei_app_picking_list(), *((uint32_t*) &color));
    hw_surface_unlock(pick_surface);
    return result;
}

void ei_frame_configure (ei_widget_t* widget,
                            ei_size_t* requested_size,
                            const ei_color_t* color,
                            int* border_width,
                            ei_relief_t* relief,
                            char** text,
                            ei_font_t* text_font,
                            ei_color_t* text_color,
                            ei_anchor_t* text_anchor,
                            ei_surface_t* img,
                            ei_rect_t** img_rect,
                            ei_anchor_t* img_anchor) {
    if(color)
        ((ei_frame_t*) widget)->bg_color = *color;
    if(border_width)
        ((ei_frame_t*) widget)->border_width = *border_width;

    if(relief)
        ((ei_frame_t*) widget)->relief = *relief;
    assert(!(text && img));
    if(text) {
        if((((ei_frame_t*) widget)->opt_type != TEXT)) {
            ((ei_frame_t*) widget)->opt_type = TEXT;
            ((ei_frame_t*) widget)->opt.txt.font = ei_default_font;
            ((ei_frame_t*) widget)->opt.txt.text_color = ei_font_default_color;
            ((ei_frame_t*) widget)->opt.txt.text_anchor = ei_anc_center;
        }

        ((ei_frame_t*) widget)->opt.txt.text = malloc(strlen(*text) + 1);
        strcpy(((ei_frame_t*) widget)->opt.txt.text, *text);
        if(text_font)
            ((ei_frame_t*) widget)->opt.txt.font = *text_font;
        if(text_color)
            ((ei_frame_t*) widget)->opt.txt.text_color = *text_color;
        if(text_anchor)
            ((ei_frame_t*) widget)->opt.txt.text_anchor = *text_anchor;
    }
    else if(img) {
        if((((ei_frame_t*) widget)->opt_type != IMAGE)) {
            ((ei_frame_t*) widget)->opt_type = IMAGE;
            ((ei_frame_t*) widget)->opt.img.img_rect = NULL;
            ((ei_frame_t*) widget)->opt.img.img_anchor = ei_anc_center;
        }
        ei_size_t size = hw_surface_get_size(*img);
        ((ei_frame_t*) widget)->opt.img.img = hw_surface_create(*img, &size, EI_FALSE);
        assert(ei_copy_surface(((ei_frame_t*) widget)->opt.img.img,
                                NULL,
                                *img,
                                NULL,
                                EI_FALSE) == 0);
        if(img_rect) {
            ((ei_frame_t*) widget)->opt.img.img_rect = malloc(sizeof(ei_rect_t));
            *((ei_frame_t*) widget)->opt.img.img_rect = **img_rect;
        }
        if(img_anchor)
            ((ei_frame_t*) widget)->opt.img.img_anchor = *img_anchor;
    }
    if(requested_size)
        widget->requested_size = *requested_size;
    else if(widget->requested_size.width == 0 && widget->requested_size.height == 0) {
            if(((ei_frame_t*) widget)->opt_type == TEXT)
                hw_text_compute_size(((ei_frame_t*) widget)->opt.txt.text,
                                     ((ei_frame_t*) widget)->opt.txt.font,
                                     &widget->requested_size.width,
                                     &widget->requested_size.height);
        widget->requested_size.width += ((ei_frame_t*) widget)->border_width * 2;
        widget->requested_size.height += ((ei_frame_t*) widget)->border_width * 2;
        if(((ei_frame_t*) widget)->opt_type == IMAGE) {
            widget->requested_size.width += ((ei_frame_t*) widget)->opt.img.img_rect->size.width;
            widget->requested_size.height += ((ei_frame_t*) widget)->opt.img.img_rect->size.height;
        }
    }
}

void ei_button_configure(ei_widget_t* widget,
                            ei_size_t* requested_size,
                            const ei_color_t* color,
                            int* border_width,
                            int* corner_radius,
                            ei_relief_t* relief,
                            char** text,
                            ei_font_t* text_font,
                            ei_color_t* text_color,
                            ei_anchor_t* text_anchor,
                            ei_surface_t* img,
                            ei_rect_t** img_rect,
                            ei_anchor_t* img_anchor,
                            ei_callback_t* callback,
                            void** user_param) {
    ei_frame_configure(widget, requested_size, color, border_width, relief,
            text, text_font, text_color, text_anchor, img, img_rect, img_anchor);
    if (corner_radius) {
        int min_size = (widget->requested_size.height < widget->requested_size.width) ?
                        widget->requested_size.height : widget->requested_size.width;
        if (*corner_radius * 2 > min_size)
            ((ei_button_t*) widget)->corner_radius = min_size/2;
        else ((ei_button_t*) widget)->corner_radius = *corner_radius;
        int offset = ((ei_button_t*) widget)->corner_radius * (1 -(1 - sqrt(2)/2));
        widget->requested_size.width += offset;
        widget->requested_size.height += offset;
    }
    if (callback)
        ((ei_button_t*) widget)->callback = *callback;
    if (user_param)
        ((ei_button_t*) widget)->user_param = *user_param;
}

void ei_toplevel_configure (ei_widget_t* widget,
							 ei_size_t* requested_size,
							 ei_color_t* color,
							 int* border_width,
							 char** title,
							 ei_bool_t* closable,
							 ei_axis_set_t* resizable,
						 	 ei_size_t** min_size) {
    if (requested_size)
        widget->requested_size = *requested_size;
    if (color)
        ((ei_toplevel_t*) widget)->bg_color = *color;
    if (border_width)
        ((ei_toplevel_t*) widget)->border_width = *border_width;
    if (title)
        ((ei_toplevel_t*) widget)->title = *title;
    if (closable)
        ((ei_toplevel_t*) widget)->closable = *closable;
    if (resizable)
        ((ei_toplevel_t*) widget)->resizable = *resizable;
    if (min_size)
        ((ei_toplevel_t*) widget)->min_size = *min_size;

    // if (requested_size || border_width) 
        widget->screen_location.size.width = widget->requested_size.width + 2 * ((ei_toplevel_t*) widget)->border_width;
        widget->screen_location.size.height = widget->requested_size.height + 2 * ((ei_toplevel_t*) widget)->border_width + BORDER;

    if(((ei_toplevel_t*) widget)->closable) {
        ((ei_toplevel_t*) widget)->close_button = ei_widget_create("button", ei_app_root_widget());
        ei_size_t button_size = {10, 10};
        ei_color_t button_color = {255, 0, 0, 255};
        int button_border = 2;
        int button_radius = 10;
        // ei_point_t button_location = {widget->screen_location.top_left.x + 10, widget->screen_location.top_left.y + 10};
        ei_button_configure(((ei_toplevel_t*) widget)->close_button, &button_size, &button_color, &button_border, &button_radius, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        // ei_place(((ei_toplevel_t*) widget)->close_button, NULL, &(button_location.x), &(button_location.y), NULL, NULL, NULL, NULL, NULL, NULL);
    }
    if(((ei_toplevel_t*) widget)->resizable) {
        ((ei_toplevel_t*) widget)->resize_button = ei_widget_create("button", ei_app_root_widget());
        ei_size_t button_size = {10, 10};
        int zero = 0;
        ei_color_t button_color = {((ei_toplevel_t*) widget)->bg_color.red * 0.65, ((ei_toplevel_t*) widget)->bg_color.green * 0.65, ((ei_toplevel_t*) widget)->bg_color.blue * 0.65, 255};
        // ei_point_t button_location = {widget->screen_location.top_left.x - button_size.width, widget->screen_location.top_left.y - button_size.height};
        ei_button_configure(((ei_toplevel_t*) widget)->resize_button, &button_size, &button_color, &zero, &zero, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        // ei_place(((ei_toplevel_t*) widget)->resize_button, NULL, &(button_location.x), &(button_location.y), NULL, NULL, NULL, NULL, NULL, NULL);
    }
}
