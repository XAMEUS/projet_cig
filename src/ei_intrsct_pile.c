#include <stdlib.h>
#include <assert.h>

#include "ei_intrsct_pile.h"

#include <stdio.h>


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

void ei_print_pile(ei_pile *pile) {
    printf("PILE\n");
    while(pile) {
        printf("%d %d %d %d\n", pile->rect.top_left.x, pile->rect.top_left.y, pile->rect.size.width, pile->rect.size.height);
        pile = pile->prev;
    }
    printf("\n");
}
