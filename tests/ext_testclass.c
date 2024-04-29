//
//  test_ext_class.c
//  projet_c_IG
//
//  Created by François Bérard on 1/6/16.
//  Copyright © 2016 LIG-IIHM. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_utils.h"
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"


void testclass_register();
void testclass_configure(ei_widget_t widget, int margin);
int testclass_get_margin(ei_widget_t widget);


ei_widget_t		g_test_widget;
ei_string_t		g_label_ok	= "Ok";
ei_string_t		g_label_cancel	= "Cancel";


bool default_handler(ei_widget_t widget, ei_event_t* event, void* user_param)
{
	SDL_KeyCode key_code = event->param.key_code;

	if ( (event->type == ei_ev_close) ||
	    ((event->type == ei_ev_keydown) && (key_code == SDLK_ESCAPE))) {
		ei_app_quit_request();
		return true;

	} else if ((event->type == ei_ev_keydown) && ((key_code == SDLK_UP) || (key_code == SDLK_DOWN))) {

		// increases/decreases margin

		int margin = testclass_get_margin(g_test_widget) + ((key_code == SDLK_UP) ? 1 : -1);
		if (margin < 1)
			margin = 1;
		testclass_configure(g_test_widget, margin);
		return true;

	} else
		return false;
}


bool button_callback(ei_widget_t widget, struct ei_event_t* event, ei_user_param_t user_param)
{
	static bool		flip	= false;

	ei_button_set_text(widget, flip ? g_label_ok : g_label_cancel);
	flip			= !flip;

	return true;
}



int main(int argc, char** argv)
{
        ei_widget_t	top;
        ei_size_t	top_min_size		= {50, 50};
	ei_widget_t	but;

	// Initialize the application, set the background color.

	ei_app_create			((ei_size_t){800, 800}, false);
	ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	// Register the new "testclass" widget class.

	testclass_register();

	// Create a toplevel, place it on the screen.

        top = ei_widget_create		("toplevel", ei_app_root_widget(), NULL, NULL);
        ei_toplevel_configure		(top,	&(ei_size_t){400, 400},
				      		&(ei_color_t){0xA0,0xA0,0xA0, 0xff},
				      		&(int){2},
					      	&(ei_string_t){"Test nouvelle classe de widget"},
						&(bool){"Test nouvelle classe de widget"},
						&(ei_axis_set_t){ei_axis_both},
						&(ei_size_ptr_t){&top_min_size});
	ei_place_xy			(top, 30, 10);

	// Create the testclass widget as a child of the toplevel, use relative placement.

	g_test_widget = ei_widget_create("testclass", top, NULL, NULL);
	testclass_configure(g_test_widget, 8);
	ei_place			(g_test_widget,	&(ei_anchor_t){ei_anc_center}, NULL, NULL,
					 		&(int){-40}, &(int){-40},
							 &(float){0.5f}, &(float){0.5f},
							 &(float){1.0f}, &(float){1.0f});

	// Create a button as a child of the testclass widget, place it.

	but = ei_widget_create		("button", g_test_widget, NULL, NULL);

	ei_button_configure		(but,	NULL, NULL, NULL, NULL, NULL,
					    	&(ei_string_t){g_label_ok}, NULL, NULL,
					    	&(ei_anchor_t){ei_anc_west}, NULL, NULL, NULL,
					    	&(ei_callback_t){button_callback}, NULL);
	ei_place			(but,	&(ei_anchor_t){ei_anc_center}, NULL, NULL, NULL, NULL,
					 	&(float){1.0f}, &(float){0.5f},
					 	&(float){1.0f}, NULL);

	// Run the main loop, bind the "Esc" key to quitting the application.

	ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);

        ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}


