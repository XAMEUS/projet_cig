
#ifndef EI_UTILS_H
#define EI_UTILS_H

#include "ei_types.h"
#include <stdlib.h>

ei_rect_t *ei_rect_intrsct(ei_rect_t *rect1, ei_rect_t *rect2);
ei_rect_t *ei_rect_pack(ei_rect_t *rect1, ei_rect_t *rect2);
ei_bool_t in_rect(ei_point_t *pt, ei_rect_t *rect);
char **strsplit(const char* str, const char* delim, size_t* numtokens);

#endif
