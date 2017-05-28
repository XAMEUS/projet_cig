#include "ei_application.h"
#include "ei_frame.h"
#include "ei_widgetclass.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static ei_widget_t *ROOT_WIDGET;
static ei_surface_t ROOT_SURFACE;

/**
 * \brief	Creates an application.
 *		<ul>
 *			<li> initializes the hardware (calls \ref hw_init), </li>
 *			<li> registers all classes of widget and all geometry managers, </li>
 *			<li> creates the root window (either in a system window, or the entire
 *				screen), </li>
 *			<li> creates the root widget to accress the root window. </li>
 *		</ul>
 *
 * @param	main_window_size	If "fullscreen is false, the size of the root window of the
 *					application.
 *					If "fullscreen" is true, the current monitor resolution is
 *					used as the size of the root window, this size is returned
 *					in this parameter.
 * @param	fullScreen		If true, the root window is the entire screen. Otherwise, it
 *					is a system window.
 */
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {
    hw_init();
    /* We register all classes */
    ei_frame_register_class();
    ei_toplevel_register_class();
    ei_button_register_class();
    //TODO geometry managers.
    /* création d’une surface offscreen pour la gestion du picking :*/
    // surface=hw_surface_create(const ei_surface_t root, const ei_size_t* size, 0);
    /* Root window */
    ROOT_SURFACE = hw_create_window(main_window_size, fullscreen);
    /* Root widget */
    ei_widgetclass_t *frame = ei_widgetclass_from_name("frame");
    ROOT_WIDGET = frame->allocfunc();
    ROOT_WIDGET->wclass = frame;
}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */
void ei_app_free() {
    ROOT_WIDGET->wclass->releasefunc(ROOT_WIDGET);
    free(ROOT_WIDGET);
    hw_surface_free(ROOT_SURFACE);
    hw_quit();
}

/**
 * \brief	Runs the application: enters the main event loop. Exits when
 *		\ref ei_app_quit_request is called.
 */
void ei_app_run() {
    ei_widget_t *w = ROOT_WIDGET;
    while(1) {
        if(w->placer_params || w == ROOT_WIDGET)
            w->wclass->drawfunc(w, ROOT_SURFACE, NULL, NULL);
        if(w->children_head != NULL)
            w = w->children_head;
        else if(w->next_sibling != NULL)
            w = w->next_sibling;
        else {
            while(w->parent != NULL && w->parent->next_sibling == NULL)
                w = w->parent;
            if(w->parent != NULL)
                w = w->parent->next_sibling;
            else
                break;
        }
    }
    sleep(5);
}

/**
 * \brief	Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *		update on the screen will be done at the right moment in the main loop.
 *
 * @param	rect		The rectangle to add, expressed in the root window coordinates.
 *				A copy is made, so it is safe to release the rectangle on return.
 */
void ei_app_invalidate_rect(ei_rect_t* rect) {
    return;
}

/**
 * \brief	Tells the application to quite. Is usually called by an event handler (for example
 *		when pressing the "Escape" key).
 */
void ei_app_quit_request() {
    return;
}

/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t* ei_app_root_widget() {
    return ROOT_WIDGET;
}

/**
 * \brief	Returns the surface of the root window. Used to create surfaces with similar r, g, b
 *		channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface() {
    return ROOT_SURFACE;
}
