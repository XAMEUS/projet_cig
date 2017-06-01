#include "ei_application.h"
#include "ei_frame.h"
#include "ei_widgetclass.h"
//#include "ei_debug.h"
#include "ei_picking.h"
#include "ei_event.h"
#include "ei_button.h"
#include "ei_tools.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static ei_widget_t *ROOT_WIDGET;
static ei_surface_t ROOT_SURFACE, PICKING;
static ei_bool_t SHALL_WE_CONTINUE = EI_TRUE;
static list_picking *LIST_PICKING;
static ei_linked_rect_t *INVALIDATE_RECT = NULL;

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
    ei_rect_t screen_rect = {{0, 0}, ROOT_WIDGET->requested_size};
    ei_app_invalidate_rect(&screen_rect);
}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */
void ei_app_free() {
    ei_widget_t *to_free = ROOT_WIDGET;
    ei_widget_t *tmp;
    ei_widget_t *father;
    ei_widget_t *next;
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
        to_free = (to_free->next_sibling) ? to_free->next_sibling : to_free->parent;
        free(tmp);
    }
    hw_surface_free(ROOT_SURFACE);
    hw_surface_free(PICKING);
    remove_picker(LIST_PICKING);
    hw_quit();
}

void ei_app_run() {
    struct ei_event_t* event = malloc(sizeof(struct ei_event_t));
    ei_widget_t *widget_event;
    ei_widget_t *w;
    ei_rect_t *rect_clipping;
    ei_linked_rect_t *new_invalidate;
    #ifdef DEBUG
    frequency_counter_t* fc = malloc(sizeof(frequency_counter_t));
    frequency_init(fc);
    while(1) {
    #endif
        while(SHALL_WE_CONTINUE) {
            while(INVALIDATE_RECT) {
                hw_surface_lock(ROOT_SURFACE);
                hw_surface_lock(PICKING);
                w = ROOT_WIDGET;
                while(1) {
                    if(w->placer_params &&
                        (rect_clipping = ei_rect_intrsct(w->parent->content_rect,
                                         &INVALIDATE_RECT->rect))) {
                        w->wclass->drawfunc(w, ROOT_SURFACE, PICKING, rect_clipping);
                        free(rect_clipping);
                    }
                    else if(w == ROOT_WIDGET)
                        w->wclass->drawfunc(w, ROOT_SURFACE, PICKING, &INVALIDATE_RECT->rect);

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
                hw_surface_unlock(ROOT_SURFACE);
                hw_surface_unlock(PICKING);
                hw_surface_update_rects(ROOT_SURFACE, NULL);
                new_invalidate = INVALIDATE_RECT->next;
                free(INVALIDATE_RECT);
                INVALIDATE_RECT = new_invalidate;

            }
            hw_event_wait_next(event);
            widget_event = ei_event_get_active_widget();
            if(!(widget_event) && event->type <= 6 && event->type >= 4)
                widget_event = ei_widget_pick(&(event->param.mouse.where));

            if(!widget_event || !widget_event->wclass->handlefunc(widget_event, event))
                if(ei_event_get_default_handle_func())
                    ei_event_get_default_handle_func()(event);
        }
    #ifdef DEBUG
        frequency_tick(fc);
    }
    #endif
    free(event);
}

void ei_app_invalidate_rect(ei_rect_t* rect) {
    ei_linked_rect_t *n_rect = malloc(sizeof(ei_linked_rect_t));
    n_rect->rect = *rect;
    n_rect->next = INVALIDATE_RECT;
    INVALIDATE_RECT = n_rect;
    ei_linked_rect_t *lk = INVALIDATE_RECT;
    while(lk != NULL && lk->next != NULL) {
        ei_rect_t *n_rect = ei_rect_pack(&lk->rect, &lk->next->rect);
        if(n_rect->size.width * n_rect->size.height <=
            lk->rect.size.width * lk->rect.size.height +
                lk->next->rect.size.width * lk->next->rect.size.height) {
            ei_linked_rect_t *tmp = lk->next;
            lk->next = lk->next->next;
            lk->rect = *n_rect;
            free(n_rect);
            free(tmp);
        }
        else
            lk = lk->next;
    }
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
