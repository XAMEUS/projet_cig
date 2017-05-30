#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"

// ei_widget_t* frame2;
// ei_size_t frame2_size = {50,50};
// int frame2_x = 20;
// int frame2_y = 20;
// ei_color_t frame2_color = {0x88, 0x00, 0x88, 0x88};
// ei_relief_t frame2_relief = ei_relief_raised;
// int frame2_border_width = 6;
ei_widget_t    *
test_frame(ei_widget_t * parent, ei_size_t size, int x, int y, float rel_x,
	   float rel_y, ei_color_t color, ei_relief_t relief,
	   int border, ei_anchor_t anchor)
{
    ei_widget_t    *frame;
    frame = ei_widget_create("frame", parent);
    ei_frame_configure(frame, &size, &color,
		       &border, &relief, NULL, NULL, NULL,
		       NULL, NULL, NULL, NULL);
    ei_place(frame, &anchor, &x, &y, NULL, NULL, &rel_x, &rel_y, NULL,
	     NULL);
    return frame;
}

/*
 * ei_main --
 *
 *      Main function of the application.
 */
int
ei_main(int argc, char **argv)
{
    ei_size_t       screen_size = { 600, 600 };
    ei_color_t      root_bgcol = { 0x52, 0x7f, 0xb4, 0xff };

    ei_widget_t    *frame;
    ei_size_t       frame_size = { 300, 250 };
    int             frame_x = 150;
    int             frame_y = 200;
    ei_color_t      frame_color = { 0x88, 0x88, 0x88, 0xff };
    ei_relief_t     frame_relief = ei_relief_raised;
    int             frame_border_width = 6;

    /*
     * Create the application and change the color of the background.
     */
    ei_app_create(&screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL,
		       NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /*
     * Create, configure and place the frame on screen.
     */
    frame = ei_widget_create("frame", ei_app_root_widget());
    ei_frame_configure(frame, &frame_size, &frame_color,
		       &frame_border_width, &frame_relief, NULL, NULL,
		       NULL, NULL, NULL, NULL, NULL);
    ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL,
	     NULL);

    ei_size_t       size = { 50, 50 };
    ei_color_t      color = { 0x88, 0x00, 0x00, 0x88 };
    test_frame(frame, size, 20, 20, 0.0, 0.0, color, ei_relief_raised, 6,
	       ei_anc_northwest);

    color.red = 0x88;
    color.green = 0x88;
    color.blue = 0x00;
    test_frame(frame, size, -20, -20, 1.0, 1.0, color, ei_relief_raised, 6,
	       ei_anc_southeast);

    color.red = 0x88;
    color.green = 0x00;
    color.blue = 0x88;
    test_frame(frame, size, -20, 20, 1.0, 0.0, color, ei_relief_raised, 6,
	       ei_anc_northeast);

    color.red = 0x00;
    color.green = 0x00;
    color.blue = 0x88;
    test_frame(frame, size, 20, -20, 0.0, 1.0, color, ei_relief_raised, 6,
	       ei_anc_southwest);

    color.red = 0x00;
    color.green = 0x88;
    color.blue = 0x00;
    test_frame(frame, size, 0, -20, 0.5, 1.0, color, ei_relief_raised, 6,
	       ei_anc_south);

    color.red = 0x00;
    color.green = 0x88;
    color.blue = 0x88;
    test_frame(frame, size, 0, 20, 0.5, 0.0, color, ei_relief_raised, 6,
	       ei_anc_north);

    color.red = 0x00;
    color.green = 0x88;
    color.blue = 0x88;
    test_frame(frame, size, 20, 0, 0.0, 0.5, color, ei_relief_raised, 6,
	       ei_anc_west);

    color.red = 0x88;
    color.green = 0x00;
    color.blue = 0x88;
    test_frame(frame, size, -20, 0, 1.0, 0.5, color, ei_relief_raised, 6,
	       ei_anc_east);

    color.red = 0xbb;
    color.green = 0xbb;
    color.blue = 0xbb;
    test_frame(frame, size, 0, 0, 0.5, 0.5, color, ei_relief_raised, 6,
	       ei_anc_center);

    /*
     * Run the application's main loop.
     */
    ei_app_run();

    /*
     * We just exited from the main loop. Terminate the application
     * (cleanup).
     */
    ei_app_free();

    return (EXIT_SUCCESS);
}
