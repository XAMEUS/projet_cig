#include "ei_tools.h"
#include <stdlib.h>

ei_rect_t *ei_rect_intrsct(ei_rect_t *rect1, ei_rect_t *rect2) {
    ei_rect_t *intrsct = calloc(1, sizeof(ei_rect_t));
    return intrsct;
}

ei_rect_t *ei_rect_pack(ei_rect_t *rect1, ei_rect_t *rect2) {
    ei_rect_t *pack = calloc(1, sizeof(ei_rect_t));
    pack->top_left.x = (rect1->top_left.x < rect2->top_left.x) ?
                        rect1->top_left.x : rect2->top_left.x;
    pack->size.width = (rect1->top_left.x + rect1->size.width >
                         rect2->top_left.x + rect2->size.width) ?
        rect1->top_left.x + rect1->size.width - pack->top_left.x :
        rect2->top_left.x + rect2->size.width - pack->top_left.x;
    pack->top_left.y = (rect1->top_left.y < rect2->top_left.y) ?
                        rect1->top_left.y : rect2->top_left.y;
    pack->size.height = (rect1->top_left.y + rect1->size.height >
                         rect2->top_left.y + rect2->size.height) ?
        rect1->top_left.y + rect1->size.height - pack->top_left.y :
        rect2->top_left.y + rect2->size.height - pack->top_left.y;
    return pack;
}
