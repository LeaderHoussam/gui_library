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

ei_surface_t root_window;
ei_widget_t root_widget;
extern ei_widgetclass_t* liste_des_classe;
void ei_app_create(ei_size_t main_window_size, bool fullscreen){
    hw_init();

    //enregistrement des classes:
    // pour l'instant, on enregistre  juste la classe frame

    // on initialise la liste des classes
    printf("j'ai bien fait le init");
    ei_widgetclass_t* classe_frame = init_frame_classe();
    ei_widgetclass_register(classe_frame);

    printf("j'ai reussi à ajouter la classe frame");

    //enregistrement des geometry:

    root_window =   hw_create_window(main_window_size, fullscreen);
    root_widget = ei_widget_create("frame", NULL,NULL, NULL);

}

ei_widget_t ei_app_root_widget(void) {
    return root_widget;

}

void ei_app_run(void){
    //
    ei_widget_t racine = ei_app_root_widget();
    ei_surface_t surface_principale = ei_app_root_surface();
    ei_surface_t surface_arriere = hw_surface_create(surface_principale, hw_surface_get_size(surface_principale), false);
    // on doit normalement parcourir ici la hierarchie pour
    // afficher, mais je ne la comprend pas bien encore,
    // on affihce donc notre seul frame root  d'abord pour voir
    (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, NULL);

    getchar();
}

void ei_app_free(void){
    hw_quit();
}

ei_surface_t ei_app_root_surface(void) {
    return root_window;
}