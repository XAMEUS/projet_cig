/**
* @file ei_intrsct_pile.h
*
* @brief Structure and functions for the pile of intersection rectangles. Useful for clipping.
*/

#ifndef EI_INTRSCT_PILE_H
#define EI_INTRSCT_PILE_H

#include "ei_types.h"

/**
* \brief The structure of a cell in the pile of clippers.
*/
typedef struct ei_pile {
    ei_rect_t rect;
    struct ei_pile *prev;
} ei_pile;

/**
* \brief Creates a new cell and pushs it in the pile.
*
* @param pile The pile where to push the new cell.
*
* @param rect The rectangle of the new cell.
*/
void ei_pile_push(ei_pile **pile, ei_rect_t rect);

/**
* \brief Removes the cell at the top of the pile.
*
* @param pile The pile where to remove the cell.
*/
void ei_pile_pop(ei_pile **pile);

/**
* \brief Frees the pile.
*
* @param pile The pile to free.
*/
void ei_pile_clear(ei_pile **pile);

/**
* \brief Prints all the rectangles of the cells in the pile.
*
* @param The pile to print.
*/
void ei_print_pile(ei_pile *pile);

#endif
