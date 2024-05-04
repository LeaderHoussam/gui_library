//
// Created by traorefv on 29/04/24.
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_implementation.h"


void ei_app_create(ei_size_t main_window_size, bool fullscreen){
    hw_init();

    //enregistrement des classes:
    // pour l'instant, on enregistre  juste la classe frame
    ei_widgetclass_t* classe_frame = init_frame_classe();
    ei_widgetclass_register(classe_frame);

    //enregistrement des geometry:

    ei_surface_t racine = hw_create_window(main_window_size, fullscreen);

}

ei_widget_t ei_app_root_widget(void) {
    ei_widget_t  root_widget = ei_widget_create("frame", NULL,NULL, NULL);
    return root_widget;

}

void ei_app_run(void){
    //
    ei_widget_t racine = ei_app_root_widget();
    ei_size_t taille = racine->wclass->
    racine->wclass->drawfunc(racine, hw_create_window(), hw_create_window(), &({}));

}

void ei_app_free(void){
    hw_quit();
}