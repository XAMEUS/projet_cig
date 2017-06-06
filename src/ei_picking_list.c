#include "ei_picking.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

list_picking *create_picker() {
    ei_widget_t **data = calloc(1, sizeof(ei_widget_t *));
    list_picking *list = calloc(1, sizeof(list_picking));
    list->alloc_size = 1;
    list->data = data;
    return list;
}
void add_picker(list_picking *ptr_list, ei_widget_t* new_widget) {
    uint32_t indice;
    if(ptr_list->to_add) {
        indice = ptr_list->to_add->nb;
        ptr_list->to_add = ptr_list->to_add->next;
    }
    else {
        if(ptr_list->len == ptr_list->alloc_size) {
            ptr_list->alloc_size <<= 1;
            ptr_list->data = realloc(ptr_list->data, sizeof(ptr_list->data) * ptr_list->alloc_size);
        }
        indice = ptr_list->len++;
    }
    ptr_list->data[indice] = new_widget;
    new_widget->pick_id = indice;
    new_widget->pick_color = (ei_color_t *) &(new_widget->pick_id);
    new_widget->pick_color->alpha = 0xFF;
}

void del_picker(list_picking *ptr_list, uint32_t pick_id) {
    ((ei_color_t*) &pick_id)->alpha = 0;
    assert(pick_id <= ptr_list->len);
    if(ptr_list->data[pick_id] != NULL)
        ptr_list->data[pick_id]->pick_color = NULL;
    if(pick_id == ptr_list->len) ptr_list->len--;
    else {
        chained_numbers *n_num = malloc(sizeof(chained_numbers));
        n_num->next = ptr_list->to_add;
        n_num->nb = pick_id;
        ptr_list->to_add = n_num;
    }
    ptr_list->data[pick_id] = NULL;
}

ei_widget_t* take_picker(list_picking *ptr_list, uint32_t pick_id) {
    ((ei_color_t*) (&pick_id))->alpha = 0;
    assert(pick_id <= ptr_list->len);
    return((ptr_list->data)[pick_id]);
}

void remove_picker(list_picking *ptr_list) {
    list_picking list = *ptr_list;
    while(list.to_add) {
        chained_numbers *tmp = list.to_add;
        list.to_add = list.to_add->next;
        free(tmp);
    }
    free(list.data);
    free(ptr_list);
}

void print_picker(list_picking *ptr_list) {
    assert(ptr_list != NULL);
    printf("Len: %u \n", ptr_list->len);
    printf("alloc_size: %u \n", ptr_list->alloc_size);
    for (uint32_t i = 0; i < ptr_list->alloc_size; i++) {
        printf("=> %u: %lu\n", i, (size_t) (ptr_list->data)[i]);
    }
    printf("[");
    chained_numbers *tmp = ptr_list->to_add;
    while(tmp) {
        printf("%u ", tmp->nb);
        tmp = tmp->next;
    }
    printf("]\n");
}
