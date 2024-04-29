#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_entry.h"
#include "ei_widget_configure.h"
#include "ei_widget_attributes.h"
#include "ei_placer.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"

ei_point_t		g_current_toplevel_position	= {20, 40};
const ei_point_t 	g_toplevel_position_increment	= {120, 140};


void create_login_window();

bool default_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{

	if ( (event->type == ei_ev_close) ||
	    ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_ESCAPE))) {

		ei_app_quit_request();
		return true;

	} else if ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_w) && (ei_event_has_ctrl(event))) {
		ei_widget_t last = ei_widget_get_last_child(ei_app_root_widget());
		if ((last != NULL) && (strcmp(ei_widget_get_class(last)->name, "toplevel") == 0))
			ei_widget_destroy(last);

	} else if ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_n) && (ei_event_has_ctrl(event))) {

		create_login_window();
		return true;
	}
	return false;
}

ei_size_t widget_size(ei_widget_t widget)
{
	ei_place_xy(widget, 0, 0);
	ei_rect_t wr = *ei_widget_get_screen_location(widget);
	ei_geometrymanager_unmap(widget);

	return wr.size;
}

bool button_cb(ei_widget_t widget, struct ei_event_t* event, ei_user_param_t user_param)
{
	ei_widget_t toplevel = (ei_widget_t)user_param;
	ei_widget_destroy(toplevel);
	return true;
}

void create_login_window()
{
	ei_widget_t	toplevel, label, entries[2], button;
	ei_size_t	label_size, edit_size;
	int		i, y, label_y_offset, margin = 6;

	// toplevel

	toplevel = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure		(toplevel, NULL, NULL, NULL, &(ei_string_t){"Login information"}, NULL, &(ei_axis_set_t){ei_axis_x}, NULL);
	ei_place_xy			(toplevel, g_current_toplevel_position.x, g_current_toplevel_position.y);
	g_current_toplevel_position	= ei_point_add(g_current_toplevel_position, g_toplevel_position_increment);

	// size of the longest label

	label				= ei_widget_create("frame", toplevel, NULL, NULL);
	ei_frame_set_text		(label, "Mot de passe");
	label_size			= widget_size(label);

	// 2 lines, each with a label ("Login" and "Mot de passe") and an entry.

	for (i = 0, y = margin; i < 2; i++, y += 2*margin + edit_size.height) {
		if (i == 1)
			label		= ei_widget_create("frame", toplevel, NULL, NULL);
		ei_frame_configure	(label, NULL, NULL, NULL, NULL, &(ei_string_t){ i ==  0 ? "Login" : "Mot de passe" },
					   			NULL, NULL, &(ei_anchor_t){ei_anc_east}, NULL, NULL, NULL);

		entries[i]		= ei_widget_create	("entry", toplevel, NULL, NULL);
		ei_entry_configure	(entries[i], &(int){30}, &(ei_color_t){0xff, 0xff, 0xff, 0xff}, NULL, NULL, NULL);
		if (i == 0) {
			ei_entry_set_text(entries[i], "Some login");
			edit_size	= widget_size(entries[i]);
			label_y_offset	= (edit_size.height - label_size.height) / 2;
		}

		ei_place		(label, &(ei_anchor_t){ei_anc_northeast}, &(int){label_size.width + margin},
					 			&(int){y + label_y_offset}, NULL, NULL, NULL, NULL, NULL, NULL);
		ei_place		(entries[i], &(ei_anchor_t){ei_anc_northwest}, &(int){label_size.width + 2*margin}, &y,
					 			&(int){-label_size.width - 3*margin}, NULL, NULL, NULL, &(float){1.0f}, NULL);
	}
	ei_entry_give_focus(entries[0]);

	// Ok button

	button				= ei_widget_create("button", toplevel, NULL, NULL);
	ei_button_configure		(button, NULL, NULL, NULL, NULL, NULL, &(ei_string_t){"Ok"}, NULL, NULL, NULL, NULL, NULL, NULL,
					    			&(ei_callback_t){button_cb}, &(ei_user_param_t){toplevel});
	ei_place			(button, &(ei_anchor_t){ei_anc_southeast}, &(int){-3*margin}, &(int){-margin}, &(int){60},
					 			NULL, &(float){1.0f}, &(float){1.0f}, NULL, NULL);

	// resize toplevel

	const ei_rect_t* erect = ei_widget_get_screen_location(entries[1]);
	const ei_rect_t* brect = ei_widget_get_screen_location(button);
	const ei_rect_t* trect = ei_widget_get_screen_location(toplevel);

	ei_widget_set_requested_size(toplevel, ei_size(erect->top_left.x - trect->top_left.x + erect->size.width + margin,
						       erect->top_left.y - trect->top_left.y + erect->size.height + 3*margin + brect->size.height));
}

int main(int argc, char** argv)
{
	/* Create the application and change the color of the background. */
	ei_app_create			((ei_size_t){1200, 800}, false);
	ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	create_login_window();
	create_login_window();

	/* Register the default callback. */
	ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
