#include "ei_tools.h"
#include <stdlib.h>
#include <math.h>

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

ei_rect_t *ei_rect_intrsct(ei_rect_t *rect1, ei_rect_t *rect2) {

    uint32_t x1 = rect1->top_left.x + rect1->size.width;
    uint32_t x2 = rect2->top_left.x + rect2->size.width;
    uint32_t y1 = rect1->top_left.y + rect1->size.height;
    uint32_t y2 = rect2->top_left.y + rect2->size.height;

    uint32_t x3 = max(rect1->top_left.x, rect2->top_left.x);
    uint32_t x4 = min(x1, x2);
    if (x4 <= x3)
        return NULL;
    uint32_t y3 = max(rect1->top_left.y, rect2->top_left.y);
    uint32_t y4 = min(y1, y2);
    if (y4 <= y3)
        return NULL;
    ei_rect_t *intrsct = calloc(1, sizeof(ei_rect_t));
    intrsct->top_left.x = x3;
    intrsct->top_left.y = y3;
    intrsct->size.width = x4 - x3;
    intrsct->size.height = y4 - y3;
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
