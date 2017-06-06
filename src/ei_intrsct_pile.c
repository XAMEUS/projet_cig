#include <stdlib.h>
#include <assert.h>

#include "ei_intrsct_pile.h"

#include <stdio.h>

ei_pile *ei_new_pile(ei_rect_t root_rect) {
    ei_pile* pile = malloc(sizeof(ei_pile));
    assert(pile != NULL);
    pile->rect = root_rect;
    pile->prev = NULL;
    return pile;
}

void ei_pile_push(ei_pile **pile, ei_rect_t rect) {
    ei_pile* new_element = malloc(sizeof(ei_pile));
    assert(new_element != NULL);
    new_element->rect = rect;
    new_element->prev = *pile;
    *pile = new_element;
}

void ei_pile_pop(ei_pile **pile) {
    ei_pile* prev = (*pile)->prev;
    free(*pile);
    *pile = prev;
}

void ei_pile_clear(ei_pile **pile) {
    while(*pile) {
        ei_pile_pop(pile);
    }
}
