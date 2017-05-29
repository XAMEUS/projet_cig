#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_size_t	frame_size		= {300,250};
	int		frame_x			= 150;
	int		frame_y			= 200;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure	(frame, &frame_size, &frame_color,
				 &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

	ei_widget_t*	frame2;
	ei_size_t	frame2_size		= {50,50};
	int		frame2_x			= 20;
	int		frame2_y			= 20;
	ei_color_t	frame2_color		= {0x88, 0x00, 0x88, 0x88};
	ei_relief_t	frame2_relief		= ei_relief_raised;
	int		frame2_border_width	= 6;
	frame2 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame2, &frame2_size, &frame2_color,
				 &frame2_border_width, &frame2_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame2, NULL, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, NULL, NULL );

	ei_widget_t*	frame3;
	ei_size_t	frame3_size		= {50,50};
	int		frame3_x			= -20;
	int		frame3_y			= -20;
	float		frame3_rel_x	= 1.0;
	float		frame3_rel_y	= 1.0;
	ei_color_t	frame3_color		= {0x88, 0x88, 0x00, 0x88};
	ei_relief_t	frame3_relief		= ei_relief_raised;
	int		frame3_border_width	= 6;
	ei_anchor_t frame3_anchor = ei_anc_southeast;
	frame3 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame3, &frame3_size, &frame3_color,
				 &frame3_border_width, &frame3_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame3, &frame3_anchor, &frame3_x, &frame3_y, NULL, NULL, &frame3_rel_x, &frame3_rel_y, NULL, NULL );

	ei_widget_t*	frame4;
	ei_size_t	frame4_size		= {50,50};
	int		frame4_x			= -20;
	int		frame4_y			= 20;
	float		frame4_rel_x	= 1.0;
	float		frame4_rel_y	= 0.0;
	ei_color_t	frame4_color		= {0x88, 0x00, 0x00, 0x88};
	ei_relief_t	frame4_relief		= ei_relief_raised;
	int		frame4_border_width	= 6;
	ei_anchor_t frame4_anchor = ei_anc_northeast;
	frame4 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame4, &frame4_size, &frame4_color,
				 &frame4_border_width, &frame4_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame4, &frame4_anchor, &frame4_x, &frame4_y, NULL, NULL, &frame4_rel_x, &frame4_rel_y, NULL, NULL );

	ei_widget_t*	frame5;
	ei_size_t	frame5_size		= {50,50};
	int		frame5_x			= 20;
	int		frame5_y			= -20;
	float		frame5_rel_x	= 0.0;
	float		frame5_rel_y	= 1.0;
	ei_color_t	frame5_color		= {0x00, 0x00, 0x88, 0x88};
	ei_relief_t	frame5_relief		= ei_relief_raised;
	int		frame5_border_width	= 6;
	ei_anchor_t frame5_anchor = ei_anc_southwest;
	frame5 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame5, &frame5_size, &frame5_color,
				 &frame5_border_width, &frame5_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame5, &frame5_anchor, &frame5_x, &frame5_y, NULL, NULL, &frame5_rel_x, &frame5_rel_y, NULL, NULL );

	ei_widget_t*	frame6;
	ei_size_t	frame6_size		= {50,50};
	int		frame6_x			= 0;
	int		frame6_y			= -20;
	float		frame6_rel_x	= 0.5;
	float		frame6_rel_y	= 1.0;
	ei_color_t	frame6_color		= {0x00, 0x88, 0x00, 0x88};
	ei_relief_t	frame6_relief		= ei_relief_raised;
	int		frame6_border_width	= 6;
	ei_anchor_t frame6_anchor = ei_anc_south;
	frame6 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame6, &frame6_size, &frame6_color,
				 &frame6_border_width, &frame6_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame6, &frame6_anchor, &frame6_x, &frame6_y, NULL, NULL, &frame6_rel_x, &frame6_rel_y, NULL, NULL );

	ei_widget_t*	frame7;
	ei_size_t	frame7_size		= {50,50};
	int		frame7_x			= 0;
	int		frame7_y			= 20;
	float		frame7_rel_x	= 0.5;
	float		frame7_rel_y	= 0.0;
	ei_color_t	frame7_color		= {0x00, 0x88, 0x88, 0x88};
	ei_relief_t	frame7_relief		= ei_relief_raised;
	int		frame7_border_width	= 6;
	ei_anchor_t frame7_anchor = ei_anc_north;
	frame7 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame7, &frame7_size, &frame7_color,
				 &frame7_border_width, &frame7_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame7, &frame7_anchor, &frame7_x, &frame7_y, NULL, NULL, &frame7_rel_x, &frame7_rel_y, NULL, NULL );

	ei_widget_t*	frame8;
	ei_size_t	frame8_size		= {50,50};
	int		frame8_x			= 20;
	int		frame8_y			= 0;
	float		frame8_rel_x	= 0.0;
	float		frame8_rel_y	= 0.5;
	ei_color_t	frame8_color		= {0x00, 0x88, 0x88, 0x88};
	ei_relief_t	frame8_relief		= ei_relief_raised;
	int		frame8_border_width	= 6;
	ei_anchor_t frame8_anchor = ei_anc_west;
	frame8 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame8, &frame8_size, &frame8_color,
				 &frame8_border_width, &frame8_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame8, &frame8_anchor, &frame8_x, &frame8_y, NULL, NULL, &frame8_rel_x, &frame8_rel_y, NULL, NULL );

	ei_widget_t*	frame9;
	ei_size_t	frame9_size		= {50,50};
	int		frame9_x			= -20;
	int		frame9_y			= 0;
	float		frame9_rel_x	= 1.0;
	float		frame9_rel_y	= 0.5;
	ei_color_t	frame9_color	= {0x00, 0x88, 0x88, 0x88};
	ei_relief_t	frame9_relief	= ei_relief_raised;
	int		frame9_border_width	= 6;
	ei_anchor_t frame9_anchor = ei_anc_east;
	frame9 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame9, &frame9_size, &frame9_color,
				 &frame9_border_width, &frame9_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame9, &frame9_anchor, &frame9_x, &frame9_y, NULL, NULL, &frame9_rel_x, &frame9_rel_y, NULL, NULL );

	ei_widget_t*	frame10;
	ei_size_t	frame10_size		= {50,50};
	int		frame10_x			= 0;
	int		frame10_y			= 0;
	float		frame10_rel_x	= 0.5;
	float		frame10_rel_y	= 0.5;
	ei_color_t	frame10_color	= {0xff, 0xff, 0xff, 0x88};
	ei_relief_t	frame10_relief	= ei_relief_raised;
	int		frame10_border_width	= 6;
	ei_anchor_t frame10_anchor = ei_anc_center;
	frame10 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame10, &frame10_size, &frame10_color,
				 &frame10_border_width, &frame10_relief, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame10, &frame10_anchor, &frame10_x, &frame10_y, NULL, NULL, &frame10_rel_x, &frame10_rel_y, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
