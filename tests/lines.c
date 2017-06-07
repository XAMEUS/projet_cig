#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_draw_button.h"
#include "ei_draw_toplevel.h"
#include "ei_draw_radiobuttons.h"
#include "ei_types.h"

/* test_text --
 */
void test_text(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t color = {255, 0, 0, 255};
	ei_point_t where = {200, 150};
	char* text = "Hello World!";
	ei_font_t font = hw_text_font_create("misc/font.ttf", ei_style_normal, 48);

	ei_draw_text(surface, &where, text, font, &color, clipper);
	hw_text_font_free(font);
}

/* test_copy --
 */
void test_copy(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_size_t size = hw_surface_get_size(surface);
	ei_surface_t surface2 = hw_surface_create(surface, &size, EI_FALSE);
	ei_copy_surface(surface, clipper, surface2, clipper, EI_FALSE);
}

/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_line(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_linked_point_t	pts[2];

	pts[0].point.x = 200; pts[0].point.y = 200; pts[0].next = &pts[1];
	pts[1].point.x = 600; pts[1].point.y = 400; pts[1].next = NULL;

	ei_draw_polyline(surface, pts, color, clipper);
}



/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 255, 0, 255 };
	ei_linked_point_t	pts[9];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 400;
	pts[0].point.y = 90;

	/* Draw the octogone */
	for(i = 1; i <= 8; i++) {
		 /*	Add or remove 70/140 pixels for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 2 or 1, according to which coordinate grows faster
			The third term is simply the amount of pixels to skip */
		xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
		ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color, clipper);
	ei_draw_polygon(surface, pts, color, clipper);
}



/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 0, 100 };
	ei_linked_point_t	pts[5];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 300;
	pts[0].point.y = 400;

	/* Draw the square */
	for(i = 1; i <= 4; i++) {
		/*	Add or remove 200 pixels or 0 for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 0 or 1, according to which coordinate grows
			The third term is simply the side of the square */
		xdiff = pow(-1, i / 2) * (i % 2) * 200;
		ydiff = pow(-1, i / 2) * (i % 2 == 0) * 200;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color, clipper);
	ei_draw_polygon(surface, pts, color, clipper);
}



/* test_dot --
 *
 *	Draws a dot in the middle of the screen. This is meant to test the special
 *	case when dx = dy = 0
 */
void test_dot(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 0, 255 };
	ei_linked_point_t	pts[3];

	pts[0].point.x = 400; pts[0].point.y = 300; pts[0].next = &(pts[1]);
	pts[1].point.x = 400; pts[1].point.y = 300; pts[1].next = NULL;

	ei_draw_polyline(surface, pts, color, clipper);
}

void test_circle(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 255, 100 };
	ei_point_t	center = { 400, 300 };
	ei_linked_point_t* pts = NULL;
	ei_linked_point_t* last_point = arc(&pts, center, 200, 0, 2 * acos(-1));

	ei_linked_point_t* point = malloc(sizeof(ei_linked_point_t));
	assert(point != NULL);
	point->point.x = pts->point.x;
	point->point.y = pts->point.y;
	last_point->next = point;
	point->next = NULL;

	ei_draw_polygon(surface, pts, color, clipper);

	while(pts != NULL) {
		ei_linked_point_t* p = pts;
		pts = pts->next;
		free(p);
	}
}

void test_rounded_frame(ei_surface_t surface, ei_rect_t* clipper) {
	ei_color_t		color		= { 254, 153, 204, 255 };
	ei_rect_t		frame 		= {{170, 206}, {206, 40}};
	int				radius		= 25;
	ei_linked_point_t* pts 		= rounded_frame(frame, radius);

	// ei_color_t		color_bis		= { 255, 204, 229, 255 };
	// ei_linked_point_t* pts_bis		= down_rounded_frame(frame, radius, h);

	ei_draw_polygon(surface, pts, color, clipper);
	// ei_draw_polygon(surface, pts_bis, color_bis, clipper);

	while(pts != NULL) {
		ei_linked_point_t* p = pts;
		pts = pts->next;
		free(p);
	}
}

void test_draw_button(ei_surface_t surface, ei_rect_t* clipper) {
	ei_color_t		color		= { 0, 128, 255, 255 };
	ei_rect_t		frame 		= {{100, 100}, {200, 200}};
	int				radius		= 500;
	int				border		= 100;
	ei_bool_t		push		= EI_FALSE;
	ei_relief_t		relief		= ei_relief_none;
	ei_draw_button(surface, clipper, frame, radius, border, color, relief, push);
}

// void test_top_level(ei_surface_t surface, ei_rect_t* clipper) {
// 	ei_size_t bg_size = {300, 200};
// 	ei_color_t bg_color = {0x88, 0x88, 0x88, 0xff};
// 	int border_width = 2;
// 	draw_toplevel(surface, clipper, bg_size, bg_color, border_width);
// }

void test_draw_radiobutton(ei_surface_t surface, ei_rect_t* clipper) {
	ei_color_t		color		= { 0, 128, 255, 255 };
	ei_point_t		top_left	= {100, 100};
	int 			size  		= 50;
	ei_bool_t		push		= EI_TRUE;
	ei_draw_radiobutton(surface, clipper, top_left, size, color, push);
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_rect_t*		clipper_ptr	= NULL;
	// ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
	// clipper_ptr		= &clipper;

	hw_init();

	main_window = hw_create_window(&win_size, EI_FALSE);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, clipper_ptr);

	/* Draw polylines. */
	// test_octogone	(main_window, clipper_ptr);
	// test_square	(main_window, clipper_ptr);
	// test_line	(main_window, clipper_ptr);
	// test_dot	(main_window, clipper_ptr);
	// test_circle (main_window, clipper_ptr);
	// test_rounded_frame (main_window, clipper_ptr);
	// test_draw_button (main_window, clipper_ptr);
	// test_text	(main_window, clipper_ptr);
	// test_top_level (main_window, clipper_ptr);
	test_draw_radiobutton(main_window, clipper_ptr);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_quit();
	return (EXIT_SUCCESS);
}
