#include "ei_picking.h"
#include <assert.h>
#include <stdlib.h>

list_picking create_picker() {
    ei_widget_t **data = malloc(sizeof(ei_widget_t *));
    list_picking list = {0, 1, data, NULL};
    return list;
}
void add_picker(list_picking *ptr_list, ei_widget_t* new_widget) {
    list_picking list = *ptr_list;
    uint32_t indice;
    if(list.to_add) {
        indice = list.to_add->nb;
        list.to_add = list.to_add->next;
    }
    else {
        if(list.len == list.alloc_size) {
            list.alloc_size <<= 1;
            list.data = realloc(list.data, list.alloc_size);
        }
        indice = list.len++;
    }
    list.data[indice] = new_widget;
    new_widget->pick_id = indice;
    new_widget->pick_color = (ei_color_t *) &(new_widget->pick_id);
}

void del_picker(list_picking *ptr_list, uint32_t pick_id) {
    list_picking list = *ptr_list;
    assert(pick_id <= list.len && list.data[pick_id] != NULL);
    list.data[pick_id]->pick_color = NULL;
    if(pick_id == list.len) list.len--;
    else {
        chained_numbers *n_num = malloc(sizeof(chained_numbers));
        n_num->next = list.to_add;
        n_num->nb = pick_id;
        list.to_add = n_num;
    }
    list.data[pick_id] = NULL;
}

ei_widget_t* take_picker(list_picking list, uint32_t pick_id) {
    assert(pick_id <= list.len);
    return(list.data[pick_id]);
}

void remove_picker(list_picking list) {
    while(list.to_add) {
        chained_numbers *tmp = list.to_add;
        list.to_add = list.to_add->next;
        free(tmp);
    }
    free(list.data);
    list.len = 0;
    list.alloc_size = 0;
}
