#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_parser.h"


// process_key --

bool process_key(ei_widget_t widget, ei_event_t* event, void* user_param)
{
	if (event->param.key_code == SDLK_ESCAPE) {
		ei_app_quit_request();
		return true;
	}
        
        return false;
}



/*
 * main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, false);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	ei_bind(ei_ev_keydown, 		NULL, "all", process_key, NULL);

	if (ei_parse_file("tests/parsing.txt") == 0)
		printf("Error while attempting to parse \"parsing.txt\", aborting.\n");
	else
		/* Run the application's main loop. */
		ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
