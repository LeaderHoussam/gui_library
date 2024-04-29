//
//  test_ext_gm.c
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


void ext_register_testgm_manager();
void ext_testgm(ei_widget_t widget, ei_anchor_t anchor);


// process_key --

bool process_key(ei_widget_t widget, ei_event_t* event, void* user_param)
{
	if (event->param.key_code == SDLK_ESCAPE) {
		ei_app_quit_request();
		return true;
	}
        
        return false;
}

// setup_widgets --

void setup_widgets(ei_widget_t parent)
{
	ei_widget_t			widgets[3];
	char*				btext		= "Que j'aime à faire apprendre un nombre utile aux sages";
	ei_size_t			fsize		= ei_size(160, 320);
	ei_color_t			fcolor		= { 0xff, 0xff, 0xff, 0xff };
	ei_color_t			f2color		= { 0xff, 0x00, 0x00, 0xff };
	ei_anchor_t			f2anchor	= ei_anc_center;
	float				f0_5		= 0.5;
	float				f0_8		= 0.8;

	widgets[0]			= ei_widget_create("button", parent, NULL, NULL);
	ei_button_configure(widgets[0], NULL, NULL, NULL, NULL, NULL, &btext, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	widgets[1]			= ei_widget_create("frame", parent, NULL, NULL);
	ei_frame_configure(widgets[1], &fsize, &fcolor, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	widgets[2]			= ei_widget_create("frame", widgets[1], NULL, NULL);
	ei_frame_configure(widgets[2], NULL, &f2color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	ext_testgm(widgets[0], ei_anc_northeast);
	ext_testgm(widgets[1], ei_anc_south);
	ei_place(widgets[2], &f2anchor, NULL, NULL, NULL, NULL, &f0_5, &f0_5, &f0_8, &f0_8);
}



// main --

int main(int argc, char** argv)
{
	ei_size_t	screen_size	= {800, 800};
        ei_color_t      root_bgcol      = {0x52, 0x7f, 0xb4, 0xff};
	
        ei_size_t       window_size     = {400,400};
        char*           window_title    = "Test geometry manager";
        ei_color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
        int             window_border_width    = 2;
        bool      	closable        = true;
        ei_axis_set_t   window_resizable = ei_axis_both;
        ei_point_t	window_position	 = {30, 10};        

        ei_widget_t    toplevel;

	ei_app_create(screen_size, false);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);        

	ext_register_testgm_manager();

        toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
        ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

	setup_widgets(toplevel);

	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}


