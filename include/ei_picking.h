/**
 * @file	ei_picking.h
 *
 * @brief 	Structures and functions to define and use a picking list. Useful for the picker.
 */

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
    uint32_t nb; ///< Represents the number.
    struct chained_numbers *next; ///< The next number is here.
} chained_numbers;

/**
 * \brief A list for the picking
 *
 */
typedef struct list_picking {
    uint32_t len; ///< The maximum element of the list.
    uint32_t alloc_size; ///< The number of allocated elements.
    ei_widget_t **data; ///< The list.
    chained_numbers *to_add; ///< Blank elements in the picking list.
} list_picking;

/**
 * \brief	Returns the "picking list" of the application.
 * @return 			The picking list.
 */
list_picking* ei_app_picking_list();

/**
 * \brief	Creates the "picking list" of the application.
 * @return 			The picking list.
 */
list_picking *create_picker();

/**
 * \brief	Adds to the picking list a new widget.
 * @param	ptr_list		The pointer of the picking list.
 * @param   new_widget      The widget that will be added.
 */
void add_picker(list_picking *ptr_list, ei_widget_t* new_widget);

/**
 * \brief	Removes to the picking list the widget with the "color" pick_id.
 * @param	ptr_list		The pointer of the picking list.
 * @param   pick_id         The "color" of the widget.
 */
void del_picker(list_picking *ptr_list, uint32_t pick_id);

/**
 * \brief	Returns the widget that has the right pick_id.
 * @param	ptr_list		The pointer of the picking list.
 * @param   pick_id         The "color" of the widget.
 * @return 			The widget.
 */
ei_widget_t* take_picker(list_picking *ptr_list, uint32_t pick_id);

/**
 * \brief	Destroys the "picking list" of the application.
 * @param	ptr_list		The pointer of the picking list.
 */
void remove_picker(list_picking *ptr_list);

/**
 * \brief	Prints the "picking list" of the application.
 * @param	ptr_list		The pointer of the picking list.
 */
void print_picker(list_picking *ptr_list);
#endif
