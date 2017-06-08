typedef struct ei_chained_cb {
    ei_widget_t* widget;
    ei_callback_t callback;
    void* user_param;
    struct ei_chained_cb *next;
} ei_chained_cb;
