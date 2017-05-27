#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_types.h"
#include <stdlib.h>
#include <assert.h>

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
    return NULL;
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
    return NULL;
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
    return;
    if(requested_size)
        widget->requested_size = *requested_size;
    else if(widget->requested_size.width == 0 && widget->requested_size.height == 0)
        return; //TODO

    if(color)
        ((ei_frame_t*) widget)->bg_color = *color;
    else if(((ei_frame_t*) widget)->bg_color.red == 0 &&
            ((ei_frame_t*) widget)->bg_color.green == 0 &&
            ((ei_frame_t*) widget)->bg_color.blue == 0 &&
            ((ei_frame_t*) widget)->bg_color.alpha == 0)
        ((ei_frame_t*) widget)->bg_color = ei_default_background_color;
    if(border_width)
        ((ei_frame_t*) widget)->border_width = *border_width;

    if(relief)
        ((ei_frame_t*) widget)->relief = *relief;
    else if(((ei_frame_t*) widget)->relief == 0)
        ((ei_frame_t*) widget)->relief = ei_relief_none;

    if(text)
        ((ei_frame_t*) widget)->text = *text;
    if(text_font)
        ((ei_frame_t*) widget)->font = *text_font;
    else if(((ei_frame_t*) widget)->font == NULL)
        ((ei_frame_t*) widget)->font = ei_default_font;
    if(text_color)
        ((ei_frame_t*) widget)->text_color = *text_color;
    else if(((ei_frame_t*) widget)->text_color.red == 0 &&
            ((ei_frame_t*) widget)->text_color.green == 0 &&
            ((ei_frame_t*) widget)->text_color.blue == 0 &&
            ((ei_frame_t*) widget)->text_color.alpha == 0)
        ((ei_frame_t*) widget)->text_color = ei_font_default_color;
    if(text_anchor)
        ((ei_frame_t*) widget)->text_anchor = *text_anchor;
    else if(((ei_frame_t*) widget)->text_anchor == 0)
        ((ei_frame_t*) widget)->text_anchor = ei_anc_center;

    if(img)
        ((ei_frame_t*) widget)->img = *img;
    if(img_rect)
        ((ei_frame_t*) widget)->img_rect = *img_rect;
    if(img_anchor)
        ((ei_frame_t*) widget)->img_anchor = *img_anchor;
    else if(((ei_frame_t*) widget)->img_anchor == 0)
        ((ei_frame_t*) widget)->img_anchor = ei_anc_center;
    assert(!((((ei_frame_t*) widget)->img != NULL) && (((ei_frame_t*) widget)->text != NULL)));
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
    return;
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
    return;
}
