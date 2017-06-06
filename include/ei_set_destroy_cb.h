#ifndef EI_DESTROY_CB
#define EI_DESTROY_CB
#include "ei_types.h"
#include "ei_widget.h"

// ei_widget_set_destroy_cb --
//
//	An undocumented function that registers a callback to be called when a widget
//	is about to be destroyed.
//	This must be implemented in the library for the "two048" program to compile and
//	execute correctly.

void ei_widget_set_destroy_cb	(ei_widget_t*		widget,
				 ei_callback_t		callback,
				 void*			user_param);


 typedef struct ei_chained_cb {
     ei_widget_t* widget;
     ei_callback_t callback;
     void* user_param;
     struct ei_chained_cb *next;
 } ei_chained_cb;

void ei_widget_remove_cb();

#endif
