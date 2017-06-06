#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_set_destroy_cb.h"
#include "ei_picking.h"
#include "ei_application.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static ei_chained_cb *CHAINED_CB = NULL;

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
    ei_chained_cb *tmp_list = CHAINED_CB;
    if(CHAINED_CB) {
        if(CHAINED_CB->widget == widget) {
            CHAINED_CB = tmp_list->next;
            tmp_list->callback(tmp_list->widget, NULL, tmp_list->user_param);
            free(tmp_list);
        }
        else while(tmp_list->next != NULL) {
            if(tmp_list->next->widget == widget) {
                tmp_list->next->callback(tmp_list->next->widget, NULL, tmp_list->next->user_param);
                ei_chained_cb *next = tmp_list->next->next;
                free(tmp_list->next);
                tmp_list->next = next;
                break;
            }
            tmp_list = tmp_list->next;
        }
    }
    ei_widget_t *to_free = widget;
    ei_widget_t *tmp;
    while(to_free) {
        while(to_free->children_head)
            to_free = to_free->children_head;

        if(to_free->parent) {
            if(to_free->parent->children_head == to_free) {
                to_free->parent->children_head = to_free->next_sibling;
                if(to_free->parent->children_tail == to_free)
                    to_free->parent->children_tail = NULL;
            }
            else for(tmp = to_free->parent->children_head;
                     tmp->next_sibling != NULL;
                     tmp = tmp->next_sibling) {
                if(tmp->next_sibling == to_free) {
                    tmp->next_sibling = to_free->next_sibling;
                    if(to_free->parent->children_tail == to_free)
                        to_free->parent->children_tail = tmp;
                break;
                }
            }
        }
        del_picker(ei_app_picking_list(), widget->pick_id);
        tmp = to_free;
        ei_placer_forget(tmp);
        if(tmp->content_rect && tmp->content_rect != &tmp->screen_location)
            free(tmp->content_rect);
        if(to_free == widget)
            to_free = NULL;
        else if(to_free->next_sibling)
            to_free = to_free->next_sibling;
        else
            to_free = to_free->parent;
        tmp->wclass->releasefunc(tmp);
        ei_app_invalidate_rect(&tmp->screen_location);
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
    ei_color_t color = {n_buff[r], n_buff[g], n_buff[b], 0x00};
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
    if(requested_size) {
        widget->requested_size = *requested_size;
        if(widget->placer_params)
            ei_placer_run(widget);
    }
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
        if(widget->placer_params)
            ei_placer_run(widget);
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
        if(widget->placer_params)
            ei_placer_run(widget);
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
    if (requested_size) {
        widget->requested_size = *requested_size;
        if(widget->placer_params)
            ei_placer_run(widget);
    }
    if (color)
        ((ei_toplevel_t*) widget)->bg_color = *color;
    if (border_width)
        ((ei_toplevel_t*) widget)->border_width = *border_width;
    if (title) {
        ((ei_toplevel_t*) widget)->title = malloc(strlen(*title) + 1);
        strcpy(((ei_toplevel_t*) widget)->title, *title);
    }
    if (closable)
        ((ei_toplevel_t*) widget)->closable = *closable;
    if (resizable)
        ((ei_toplevel_t*) widget)->resizable = *resizable;
    if (min_size) {
        ((ei_toplevel_t*) widget)->min_size = malloc(sizeof(ei_size_t));
        *(((ei_toplevel_t*) widget)->min_size) = **min_size;
    }
}

void ei_widget_set_destroy_cb	(ei_widget_t*		widget,
				 ei_callback_t		callback,
				 void*			user_param) {
    ei_chained_cb *list = CHAINED_CB;
    while(list != NULL && list->next != NULL) {
        if(list->widget == widget) {
            list->callback = callback;
            list->user_param = user_param;
            return;
        }
    }
    /* widget not in the list */
    CHAINED_CB = malloc(sizeof(ei_chained_cb));
    CHAINED_CB->widget = widget;
    CHAINED_CB->callback = callback;
    CHAINED_CB->user_param = user_param;
    CHAINED_CB->next = list;
}

void ei_widget_remove_cb() {
    ei_chained_cb *to_remove;
    while(CHAINED_CB) {
        to_remove = CHAINED_CB;
        CHAINED_CB = CHAINED_CB->next;
        free(to_remove);
    }
}
