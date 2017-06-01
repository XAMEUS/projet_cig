#ifndef EI_DRAW_BUTTON_H
#define EI_DRAW_BUTTON_H

#define M_PI    acos(-1)


ei_linked_point_t* arc(ei_linked_point_t** first_point,
                       const ei_point_t center,
                       const int radius,
                       const float first_angle,
                       const float last_angle);

ei_linked_point_t* rounded_frame(ei_rect_t frame,
                                int radius);

ei_linked_point_t* up_rounded_frame(ei_rect_t frame,
                                    int radius,
                                    int h);

ei_linked_point_t* down_rounded_frame(ei_rect_t frame,
                                      int radius,
                                      int h);

void free_linked_point(ei_linked_point_t* first_point);

void ei_draw_button(ei_surface_t surface,
                  ei_rect_t* clipper,
                  ei_rect_t frame,
                  int radius,
                  int border,
                  ei_color_t color,
                  ei_relief_t relief,
                  ei_bool_t push);


#endif
