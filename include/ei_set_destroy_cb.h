/**
 * @file	ei_set_destroy_cb.h
 *
 * @brief 	API for widgets management: creation, configuration, hierarchy, redisplay.
 */

#ifndef EI_DESTROY_CB
#define EI_DESTROY_CB
#include "ei_types.h"
#include "ei_widget.h"

/**
 * @brief	Register or update a new callback funtion with it's parameters for
 *		the widget. The function will be called we the widget will be about to be
 *		destroyed.
 *
 * @param	widget		The widget.
 * @param	callback	The callback function to call when the user clicks on the button.
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 */
void ei_widget_set_destroy_cb	(ei_widget_t*		widget,
				 ei_callback_t		callback,
				 void*			user_param);


/**
* \brief	A chained list of functions (with params) associed to the good widget.
*/
 typedef struct ei_chained_cb {
     ei_widget_t* widget; ///< If this widget is about to be destroyed, we are in the good cell
     ei_callback_t callback; ///< The function that will be called.
     void* user_param; ///< The args of the function.
     struct ei_chained_cb *next; ///< The next widget.
 } ei_chained_cb;

/**
 * \brief	Free the callback list.
 */
void ei_widget_remove_cb();

#endif
