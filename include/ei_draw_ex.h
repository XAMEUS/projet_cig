#ifndef EI_DRAW_EX_H
#define EI_DRAW_EX_H
#include "ei_types.h"
#include "hw_interface.h"

#define M_PI    acos(-1)

ei_linked_point_t* arc(ei_linked_point_t** first_point,
                       const ei_point_t center,
                       const int radius,
                       const float first_angle,
                       const float last_angle);

ei_linked_point_t* rounded_frame(ei_rect_t frame,
                                int radius);



#endif
