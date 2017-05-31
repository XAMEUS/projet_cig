#include "ei_application.h"
#include "ei_frame.h"
#include "ei_widgetclass.h"
//#include "ei_debug.h"
#include "ei_picking.h"
#include "ei_event.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ei_button.h"

static ei_widget_t *ROOT_WIDGET;
static ei_surface_t ROOT_SURFACE, PICKING;
static ei_bool_t SHALL_WE_CONTINUE = EI_TRUE;
static list_picking *LIST_PICKING;

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
    /* Root window */
    ROOT_SURFACE = hw_create_window(main_window_size, fullscreen);
    ei_size_t first = hw_surface_get_size(ROOT_SURFACE);
    /* Offscreen surface for the picking */
    LIST_PICKING = create_picker();
    PICKING = hw_surface_create(ROOT_SURFACE, &first, EI_FALSE);
    /* Root widget */
    ei_widgetclass_t *frame = ei_widgetclass_from_name("frame");
    ROOT_WIDGET = frame->allocfunc();
    ROOT_WIDGET->wclass = frame;
    ROOT_WIDGET->requested_size = hw_surface_get_size(ROOT_SURFACE);
    ROOT_WIDGET->screen_location.size = hw_surface_get_size(ROOT_SURFACE);
    ROOT_WIDGET->content_rect = &(ROOT_WIDGET->screen_location);
    add_picker(LIST_PICKING, ROOT_WIDGET);
    ei_event_set_active_widget(NULL);
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

void ei_app_run() {
    #ifdef DEBUG
    frequency_counter_t* fc = malloc(sizeof(frequency_counter_t));
    frequency_init(fc);
    while(1) {
    #endif
        ei_widget_t *w = ROOT_WIDGET;
        while(1) {
            if(w->placer_params)
                w->wclass->drawfunc(w, ROOT_SURFACE, PICKING, w->parent->content_rect);
            else if(w == ROOT_WIDGET)
                w->wclass->drawfunc(w, ROOT_SURFACE, PICKING, NULL);
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
        struct ei_event_t* event = malloc(sizeof(struct ei_event_t));
        ei_widget_t *widget;
        while(SHALL_WE_CONTINUE) {
            //TODO redessin des zones 3.7
            hw_event_wait_next(event);
            widget = ei_event_get_active_widget();
            if(!(widget) && event->type <= 6 && event->type >= 4)
                widget = ei_widget_pick(&(event->param.mouse.where));
            //We execute event
            printf("aaaaaaaaaa%u \n", widget);
            if(!widget || !widget->wclass->handlefunc(widget, event)) {
                printf("Handlefunc: échec\n");
                if(!ei_event_get_default_handle_func(event))
                    printf("Defaultfunc: échec\n");
            }
        }
    #ifdef DEBUG
        frequency_tick(fc);
    }
    #endif
}

void ei_app_invalidate_rect(ei_rect_t* rect) {
    return;
}

void ei_app_quit_request() {
    SHALL_WE_CONTINUE = EI_FALSE;
}

ei_widget_t* ei_app_root_widget() {
    return ROOT_WIDGET;
}

ei_surface_t ei_app_root_surface() {
    return ROOT_SURFACE;
}

ei_surface_t ei_app_picking_object() {
    return PICKING;
}

list_picking* ei_app_picking_list() {
    return LIST_PICKING;
}
