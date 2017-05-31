#ifndef EI_PICKING_H
#define EI_PICKING_H

#include "hw_interface.h"
#include "ei_widget.h"

/**
 * \brief	Returns the "picking surface" of the application.
 * @return 			The picking surface.
 */
ei_surface_t ei_app_picking_object();

/**
 * \brief A chained list of numbers.
 * Useful for list_picking
 */
typedef struct chained_numbers {
    uint32_t nb;
    struct chained_numbers *next;
} chained_numbers;

/**
 * \brief A list for the picking
 *
 */
typedef struct list_picking {
    uint32_t len; ///< The maximum element of the list
    uint32_t alloc_size; ///< The number of allocated elements
    ei_widget_t **data; ///< The list
    chained_numbers *to_add; ///< Blank elements in the picking list.
} list_picking;

/**
 * \brief	Returns the "picking list" of the application.
 * @return 			The picking list.
 */
list_picking* ei_app_picking_list();


list_picking *create_picker();
void add_picker(list_picking *ptr_list, ei_widget_t* new_widget);
void del_picker(list_picking *ptr_list, uint32_t pick_id);
ei_widget_t* take_picker(list_picking list, uint32_t pick_id);
void remove_picker(list_picking *ptr_list);
void print_picker(list_picking *ptr_list);
#endif
