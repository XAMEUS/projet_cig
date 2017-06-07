#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"

#include "ei_radiobutton.h"


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

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	radiobutton;
	ei_size_t	radiobutton_size		= {300,200};
	int		radiobutton_corner_radius	= 20;
	int		radiobutton_x		= 150;
	int		radiobutton_y		= 200;
	ei_color_t	radiobutton_color		= {0x88, 0x88, 0x88, 0xff};
	char*		radiobutton_text		= "Bouton";
	ei_color_t	radiobutton_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_relief_t	radiobutton_relief		= ei_relief_raised;
	int		radiobutton_border_width	= 6;
	//ei_callback_t	radiobutton_callback 	= radiobutton_press;

	ei_anchor_t radiobutton_text_anchor = ei_anc_north;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_event_set_default_handle_func(process_key);

	/* Create, configure and place the radiobutton on screen. */
	radiobutton = ei_widget_create("radiobutton", ei_app_root_widget());
	ei_radiobutton_configure(radiobutton, &radiobutton_size,
	                            &radiobutton_color,
	                            &radiobutton_text,
	                            NULL,
	                            &radiobutton_text_color,
	                            &radiobutton_text_anchor);
	//ei_radiobutton_configure(...)
	ei_color_t bg_color = {255, 0, 0, 255};
	char* text = "hello";
	ei_add_radiobutton(radiobutton, &bg_color, NULL, &text, NULL, NULL, NULL);
	// TODO: ei_radio_add_choice(radiobutton, "choix1");

	/* Hook the keypress callback to the event. */
	ei_frame_configure(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
