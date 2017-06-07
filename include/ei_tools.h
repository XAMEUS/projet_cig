/**
 *  @file	ei_tools.h
 *  @brief	Some useful tools...
 *
 */

#ifndef EI_UTILS_H
#define EI_UTILS_H

#include "ei_types.h"
#include <stdlib.h>


/**
 * \brief	Calculates the intersection of two rectangles.
 *
 * @param	rect1     the first rectangle
 * @param	rect2     the second rectangle
 *
 * @return 			the rect corresponding to the intersection (or NULL if there is no intersection).
 */
ei_rect_t *ei_rect_intrsct(ei_rect_t *rect1, ei_rect_t *rect2);


/**
 * \brief	Calculates the rectangle ('pack') containing two rectangles.
 *
 * @param	rect1     the first rectangle
 * @param	rect2     the second rectangle
 *
 * @return 			the rect corresponding to the pack.
 */
ei_rect_t *ei_rect_pack(ei_rect_t *rect1, ei_rect_t *rect2);


/**
 * \brief	Checks if a rectangle contains a sp.
 *
 * @param	pt     the point
 * @param	rect     the rectangle
 *
 * @return 			EI_TRUE if rect contains pt
 */
ei_bool_t in_rect(ei_point_t *pt, ei_rect_t *rect);

/**
 * \brief	Splits a string acording to the specified delimiter.
 *
 * @param	str     the text to split.
 * @param	rect     the delimiter.
 * @param	rect     number of tokens.
 *
 * @return 			a pointer to the first token.
 */
char **strsplit(const char* str, const char* delim, size_t* numtokens);

#endif
