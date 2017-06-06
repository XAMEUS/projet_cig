#ifndef EI_INTRSCT_PILE_H
#define EI_INTRSCT_PILE_H

#include "ei_types.h"

typedef struct ei_pile {
    ei_rect_t rect;
    struct ei_pile *prev;
} ei_pile;

ei_pile *ei_new_pile(ei_rect_t root_rect);

void ei_pile_push(ei_pile **pile, ei_rect_t rect);

void ei_pile_pop(ei_pile **pile);

void ei_pile_clear(ei_pile **pile);

#endif
