#include "ei_tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

ei_rect_t *ei_rect_intrsct(ei_rect_t *rect1, ei_rect_t *rect2) {

    long x1 = rect1->top_left.x + rect1->size.width;
    long x2 = rect2->top_left.x + rect2->size.width;
    long y1 = rect1->top_left.y + rect1->size.height;
    long y2 = rect2->top_left.y + rect2->size.height;

    long x3 = max(rect1->top_left.x, rect2->top_left.x);
    long x4 = min(x1, x2);
    if (x4 <= x3)
        return NULL;
    long y3 = max(rect1->top_left.y, rect2->top_left.y);
    long y4 = min(y1, y2);
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

ei_bool_t in_rect(ei_point_t *pt, ei_rect_t *rect) {
    return(pt->x >= rect->top_left.x &&
            pt->y >= rect->top_left.y &&
            pt->x <= rect->top_left.x + rect->size.width &&
            pt->y <= rect->top_left.y + rect->size.height);
}

char *strdup (const char *s);
char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

char* strtok_r(char *str, const char *delim, char **nextp);
char* strtok_r(char *str, const char *delim, char **nextp) {
    char *ret;
    if (str == NULL)
    {
        str = *nextp;
    }
    str += strspn(str, delim);
    if (*str == '\0')
    {
        return NULL;
    }
    ret = str;
    str += strcspn(str, delim);
    if (*str)
    {
        *str++ = '\0';
    }
    *nextp = str;
    return ret;
}

// see: https://www.quora.com/How-do-you-write-a-C-program-to-split-a-string-by-a-delimiter
char **strsplit(const char* str, const char* delim, size_t* numtokens) {
    // copy the original string so that we don't overwrite parts of it
    // (don't do this if you don't need to keep the old line,
    // as this is less efficient)
    char *s = strdup(str);
    // these three variables are part of a very common idiom to
    // implement a dynamically-growing array
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *strtok_ctx;
    for (token = strtok_r(s, delim, &strtok_ctx);
            token != NULL;
            token = strtok_r(NULL, delim, &strtok_ctx)) {
        // check if we need to allocate more space for tokens
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }
    // cleanup
    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
}
