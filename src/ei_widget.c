#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_picking.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		an existing widget.
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		The widget should be released by calling \ref ei_widget_destroy when no more needed.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can not be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */
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

/**
 * @brief	Destroys a widget. Removes it from screen if it is managed by a geometry manager.
 *		Destroys all its descendants.
 *
 * @param	widget		The widget that is to be destroyed.
 */
void ei_widget_destroy (ei_widget_t* widget) {
    return;
}


/**
 * @brief	Returns the widget that is at a given location on screen.
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 *
 * @return			The top-most widget at this location, or NULL if there is no widget
 *				at this location (except for the root widget).
 */
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


/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including borders.
 *				The geometry manager may override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the border and the text or the image. This may be (0, 0)
 *				if the widget has border_width=0, and no text and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the text.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
 				Defaults to NULL.
 * @param	img_rect	If not NULL, this rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
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

/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
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

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title diplayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to \ref EI_TRUE.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size. The default minimum
 *				size of a toplevel is (160, 120). If *min_size is NULL, this requires
 *				the toplevel to be configured to its default size.
 */
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
}
