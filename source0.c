typedef union ei_text_or_img {
    struct ei_text txt;
    struct ei_image img;
} ei_text_or_img;
typedef enum {
    NONE = 0,
    TEXT = 1,
    IMAGE = 2
} ei_opt_type;
typedef struct ei_frame_t {
    ei_widget_t widget;
    ei_relief_t relief;
    ei_color_t bg_color;
    int border_width;
    ei_opt_type opt_type;
    ei_text_or_img opt;
} ei_frame_t;
