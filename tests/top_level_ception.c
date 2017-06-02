#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_toplevel.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click ! %s from %s\n", ((ei_frame_t*) widget)->opt.txt.text, ((ei_toplevel_t *) (widget->parent))->title);
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_event_t* event)
{
	if (event->type == ei_ev_keydown)
		if (event->param.key.key_sym == SDLK_ESCAPE) {
			ei_app_quit_request();
			return EI_TRUE;
		}

	return EI_FALSE;
}

void test_simple() {

		ei_widget_t*	window;
		ei_size_t	window_size		= {500,450};
		char*		window_title		= "Parent1";
		ei_color_t	window_color		= {0xC4	,0x4D,0x58, 0xff};
		int		window_border_width	= 2;
		ei_bool_t	window_closable		= EI_TRUE;
		ei_axis_set_t	window_resizable	= ei_axis_both;
		ei_point_t	window_position		= {30, 10};

		/* Create, configure and place a toplevel window on screen. */
		window = ei_widget_create("toplevel", ei_app_root_widget());
		ei_toplevel_configure(window, &window_size, &window_color, &window_border_width,
					&window_title, &window_closable, &window_resizable, NULL);
		ei_place(window, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

		ei_widget_t*	window2;
		ei_size_t	window2_size		= {200,100};
		char*		window2_title		= "Child1_nw";
		ei_color_t	window2_color		= {0x4E, 0xCD, 0xC4, 0xff};
		int		window2_border_width	= 2;
		ei_bool_t	window2_closable		= EI_TRUE;
		ei_axis_set_t	window2_resizable	= ei_axis_both;
		ei_point_t	window2_position		= {200, 200};
		window2 = ei_widget_create("toplevel", window);
		ei_toplevel_configure(window2, &window2_size, &window2_color, &window2_border_width,
					&window2_title, &window2_closable, &window2_resizable, NULL);
		ei_place(window2, NULL, &(window2_position.x), &(window2_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

		ei_widget_t*	button;
		ei_anchor_t	button_anchor		= ei_anc_southeast;
		int		button_x		= -20;
		int		button_y		= -20;
		float		button_rel_x		= 1.0;
		float		button_rel_y		= 1.0;
		float		button_rel_width	= 0.5;
		ei_color_t	button_color		= {0xF6, 0xFB, 0x7A, 0xff};
		char*		button_title		= "bouton1";
		ei_color_t	button_text_color	= {0xFF, 0x6B, 0x6B, 0xff};
		ei_relief_t	button_relief		= ei_relief_raised;
		int		button_border_width	= 2;
		ei_callback_t	button_callback 	= button_press;
		/* Create, configure and place a button as a descendant of the toplevel window2. */
		button = ei_widget_create("button", window2);
		ei_button_configure	(button, NULL, &button_color,
					 &button_border_width, NULL, &button_relief, &button_title, NULL, &button_text_color, NULL,
					NULL, NULL, NULL, &button_callback, NULL);
		ei_place(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, NULL);
}

void test_advenced() {

		ei_widget_t*	window;
		ei_size_t	window_size		= {500,450};
		char*		window_title		= "Parent1";
		ei_color_t	window_color		= {0xC4	,0x4D,0x58, 0xff};
		int		window_border_width	= 2;
		ei_bool_t	window_closable		= EI_TRUE;
		ei_axis_set_t	window_resizable	= ei_axis_both;
		ei_point_t	window_position		= {30, 10};

		/* Create, configure and place a toplevel window on screen. */
		window = ei_widget_create("toplevel", ei_app_root_widget());
		ei_toplevel_configure(window, &window_size, &window_color, &window_border_width,
					&window_title, &window_closable, &window_resizable, NULL);
		ei_place(window, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

		ei_widget_t*	window2;
		ei_size_t	window2_size		= {200,100};
		char*		window2_title		= "Child1";
		ei_color_t	window2_color		= {0xC7, 0xF4, 0x64, 0xff};
		int		window2_border_width	= 2;
		ei_bool_t	window2_closable		= EI_TRUE;
		ei_axis_set_t	window2_resizable	= ei_axis_both;
		ei_point_t	window2_position		= {300, 300};
		window2 = ei_widget_create("toplevel", window);
		ei_toplevel_configure(window2, &window2_size, &window2_color, &window2_border_width,
					&window2_title, &window2_closable, &window2_resizable, NULL);
		ei_place(window2, NULL, &(window2_position.x), &(window2_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

		ei_widget_t*	window3;
		ei_size_t	window3_size		= {200,100};
		char*		window3_title		= "Child2_se";
		ei_color_t	window3_color		= {0x4E, 0xCD, 0xC4, 0xff};
		int		window3_border_width	= 2;
		ei_bool_t	window3_closable		= EI_TRUE;
		ei_axis_set_t	window3_resizable	= ei_axis_both;
		// ei_point_t	window3_position		= {0, 0};
		ei_point_t	window3_position		= {200, 200};
		window3 = ei_widget_create("toplevel", window);
		ei_toplevel_configure(window3, &window3_size, &window3_color, &window3_border_width,
					&window3_title, &window3_closable, &window3_resizable, NULL);
		ei_place(window3, NULL, &(window3_position.x), &(window3_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

		ei_widget_t*	button;
		ei_anchor_t	button_anchor		= ei_anc_southeast;
		int		button_x		= -20;
		int		button_y		= -20;
		float		button_rel_x		= 1.0;
		float		button_rel_y		= 1.0;
		float		button_rel_width	= 0.5;
		ei_color_t	button_color		= {0xF6, 0xFB, 0x7A, 0xff};
		char*		button_title		= "bouton1";
		ei_color_t	button_text_color	= {0xFF, 0x6B, 0x6B, 0xff};
		ei_relief_t	button_relief		= ei_relief_raised;
		int		button_border_width	= 2;
		ei_callback_t	button_callback 	= button_press;
		/* Create, configure and place a button as a descendant of the toplevel window2. */
		button = ei_widget_create("button", window2);
		ei_button_configure	(button, NULL, &button_color,
					 &button_border_width, NULL, &button_relief, &button_title, NULL, &button_text_color, NULL,
					NULL, NULL, NULL, &button_callback, NULL);
		ei_place(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, NULL);

}

void test_position() {

	ei_widget_t*	window;
	ei_size_t	window_size		= {500,450};
	char*		window_title		= "Parent1";
	ei_color_t	window_color		= {0xC4	,0x4D,0x58, 0xff};
	int		window_border_width	= 2;
	ei_bool_t	window_closable		= EI_TRUE;
	ei_axis_set_t	window_resizable	= ei_axis_both;
	ei_point_t	window_position		= {30, 10};

	/* Create, configure and place a toplevel window on screen. */
	window = ei_widget_create("toplevel", ei_app_root_widget());
	ei_toplevel_configure(window, &window_size, &window_color, &window_border_width,
				&window_title, &window_closable, &window_resizable, NULL);
	ei_place(window, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

	ei_widget_t*	window2;
	ei_size_t	window2_size		= {300,300};
	char*		window2_title		= "Child1";
	ei_color_t	window2_color		= {0x4E, 0xCD, 0xC4, 0xff};
	int		window2_border_width	= 2;
	ei_bool_t	window2_closable		= EI_TRUE;
	ei_axis_set_t	window2_resizable	= ei_axis_both;
	ei_point_t	window2_position		= {40, 40};
	window2 = ei_widget_create("toplevel", window);
	ei_toplevel_configure(window2, &window2_size, &window2_color, &window2_border_width,
				&window2_title, &window2_closable, &window2_resizable, NULL);
	ei_place(window2, NULL, &(window2_position.x), &(window2_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

	ei_widget_t*	button;
	ei_anchor_t	button_anchor		= ei_anc_northeast;
	int		button_x		= 20;
	int		button_y		= 20;
	float		button_rel_x		= 0.0;
	float		button_rel_y		= 0.0;
	int		button_width		= 60;
	int		button_height		= 50;
	ei_color_t	button_color		= {0xF6, 0xFB, 0x7A, 0xff};
	char*		button_title		= "northeast";
	ei_color_t	button_text_color	= {0xFF, 0x6B, 0x6B, 0xff};
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 2;
	ei_callback_t	button_callback 	= button_press;
	button = ei_widget_create("button", window2);
	ei_button_configure	(button, NULL, &button_color,
				 &button_border_width, NULL, &button_relief, &button_title, NULL, &button_text_color, NULL,
				NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, NULL, NULL);

	ei_widget_t*	button2;
	ei_anchor_t	button2_anchor		= ei_anc_southwest;
	int		button2_x		= -20;
	int		button2_y		= -20;
	float		button2_rel_x		= 1.0;
	float		button2_rel_y		= 1.0;
	int		button2_width	= 60;
	int		button2_height	= 50;
	ei_color_t	button2_color		= {0xF6, 0xFB, 0x7A, 0xff};
	char*		button2_title		= "southwest";
	ei_color_t	button2_text_color	= {0xFF, 0x6B, 0x6B, 0xff};
	ei_relief_t	button2_relief		= ei_relief_raised;
	int		button2_border_width	= 2;
	ei_callback_t	button2_callback 	= button_press;
	/* Create, configure and place a button2 as a descendant of the toplevel window2. */
	button2 = ei_widget_create("button", window2);
	ei_button_configure	(button2, NULL, &button2_color,
				 &button2_border_width, NULL, &button2_relief, &button2_title, NULL, &button2_text_color, NULL,
				NULL, NULL, NULL, &button2_callback, NULL);
	ei_place(button2, &button2_anchor, &button2_x, &button2_y, NULL, NULL, &button2_rel_x, &button2_rel_y, NULL, NULL);

	ei_widget_t*	button3;
	ei_anchor_t	button3_anchor		= ei_anc_center;
	int		button3_x		= 0;
	int		button3_y		= 0;
	float		button3_rel_x		= 0.5;
	float		button3_rel_y		= 0.5;
	int		button3_width	= 60;
	int		button3_height	= 50;
	ei_color_t	button3_color		= {0xF6, 0xFB, 0x7A, 0xff};
	char*		button3_title		= "center";
	ei_color_t	button3_text_color	= {0xFF, 0x6B, 0x6B, 0xff};
	ei_relief_t	button3_relief		= ei_relief_raised;
	int		button3_border_width	= 2;
	ei_callback_t	button3_callback 	= button_press;
	/* Create, configure and place a button3 as a descendant of the toplevel window2. */
	button3 = ei_widget_create("button", window2);
	ei_button_configure	(button3, NULL, &button3_color,
				 &button3_border_width, NULL, &button3_relief, &button3_title, NULL, &button3_text_color, NULL,
				NULL, NULL, NULL, &button3_callback, NULL);
	ei_place(button3, &button3_anchor, &button3_x, &button3_y, NULL, NULL, &button3_rel_x, &button3_rel_y, NULL, NULL);
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {800, 600};
	ei_color_t	root_bgcol		= {0x55	,0x62,0x70, 0xff};

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_event_set_default_handle_func(process_key);

	// test_simple();
	// test_advenced();
	test_position();
	/* Run the application's main loop. */
	ei_app_run();

	ei_app_free();

	return (EXIT_SUCCESS);
}
