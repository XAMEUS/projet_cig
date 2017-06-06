#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw_text_lines.h"
#include "ei_draw_button.h"
#include "ei_draw_toplevel.h"
#include "ei_types.h"

/*
 * test_text --
 */
void
test_text(ei_surface_t surface, ei_rect_t * clipper)
{
    ei_color_t      color = { 255, 0, 0, 255 };
    ei_point_t      where = { 0, 0 };
    char           *text =
	"Hello World!\nAd impossibile neno tenatur. Pigmaei gigantum humeris impositi plus quam ipsi gigantes vident. Qui codit bis legit. Bona opinio hominum tutior pecunia est.\n\tLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    ei_font_t       font =
	hw_text_font_create("misc/font.ttf", ei_style_normal, 48);

    ei_draw_text_lines(surface, &where, text, font, &color, clipper);
    hw_text_font_free(font);
}

int
ei_main(int argc, char **argv)
{
    ei_size_t       win_size = ei_size(800, 600);
    ei_surface_t    main_window = NULL;
    ei_color_t      white = { 0xff, 0xff, 0xff, 0xff };
    ei_rect_t      *clipper_ptr = NULL;
    // ei_rect_t clipper = ei_rect(ei_point(200, 150), ei_size(400, 300));
    // clipper_ptr = &clipper;

    hw_init();

    main_window = hw_create_window(&win_size, EI_FALSE);

    /*
     * Lock the drawing surface, paint it white.
     */
    hw_surface_lock(main_window);
    ei_fill(main_window, &white, clipper_ptr);

    /*
     * Draw polylines.
     */
    test_text(main_window, clipper_ptr);

    /*
     * Unlock and update the surface.
     */
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    /*
     * Wait for a character on command line.
     */
    getchar();

    hw_quit();
    return (EXIT_SUCCESS);
}
