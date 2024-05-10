//
// Created by traorefv on 29/04/24.
// modified by aharbilh on 07/05/24
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"

#include <ei_widget_configure.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_implementation.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"

ei_surface_t root_window = NULL;
ei_widget_t root_widget = NULL;
ei_linked_rect_t* surfaces_mise_a_jour = NULL;
ei_rect_t* clipper_final = NULL;

extern ei_widgetclass_t* liste_des_classe;
int min(int a, int b);
int max(int a, int b);

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
    hw_init();

    //enregistrement des classes:
    // pour l'instant, on enregistre  juste la classe frame

    // on initialise la liste des classes
    printf("j'ai bien fait le init");
    ei_widgetclass_t* classe_frame = init_frame_classe();
    ei_widgetclass_register(classe_frame);

    ei_widgetclass_t* classe_button = init_button_classe();
    ei_widgetclass_register(classe_button);

    printf("j'ai reussi à ajouter la classe frame");

    // on initialise ici les gestionnaires de geometrie
    ei_geometrymanager_t* placeur = init_placeur();
    ei_geometrymanager_register (placeur);

    //enregistrement des geometry:

    root_window =   hw_create_window(main_window_size, fullscreen);
    //hw_surface_lock(root_window);
    root_widget = ei_widget_create("frame", NULL,NULL, NULL);
    ei_size_t taille = hw_surface_get_size(root_window);
    ei_frame_configure(root_widget, &taille, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    //hw_surface_unlock(root_window);
}

ei_widget_t ei_app_root_widget(void) {
    return root_widget;

}

static bool quit_requested = false;

void ei_app_quit_request() {
    quit_requested = true;
}
/*
bool ei_app_should_quit() {
    return quit_requested;
}*/

void ei_app_run(void){
    ei_widget_t racine = ei_app_root_widget();
    const ei_surface_t surface_principale = ei_app_root_surface();
    ei_surface_t surface_arriere = hw_surface_create(surface_principale, hw_surface_get_size(surface_principale), false);

    // Redessiner la racine avant d'entrer dans la boucle des événements
    (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, racine->content_rect);
    hw_surface_update_rects(surface_principale, NULL);

    while (!quit_requested) {
        // Attendre le prochain événement utilisateur
        //ei_eventtype_t event;
        ei_event_t *event;
        hw_event_wait_next(event);

        // Traiter l'événement
        if (event->type == ei_ev_keydown) {
            // Exemple : Si la touche "Escape" est pressée, demander à quitter l'application
            if (event->param.key_code == SDLK_ESCAPE) {
                ei_app_quit_request();
            }
        } // décommenter après faire ei_widget_pick et ei_event_get_callback
        /*else if (event->type == ei_ev_mouse_buttondown) {
            // Exemple : Si un clic de souris est détecté, identifier le widget concerné et appeler son gestionnaire d'événements
            ei_widget_t widget_cible = ei_widget_pick(&event->param.mouse.where);
            if (widget_cible != NULL) {
                ei_callback_t callback = ei_event_get_callback(event->type, widget_cible, NULL);
                if (callback != NULL) {
                    callback(widget_cible, event, NULL);
                }
            }
        }*/ else if (event->type == ei_ev_close){
            ei_app_quit_request();
        }

        // Redessiner la racine avec les mises à jour
        (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, racine->content_rect);
        hw_surface_update_rects(surface_principale, NULL);
    }

    hw_surface_free(surface_arriere);
}

void ei_app_free(void){
    hw_quit();
}

ei_surface_t ei_app_root_surface(void) {
    return root_window;
}
// chaque fois qu'on rentre dans cette fonction
// on actualise le clipper final ou refaire nos dessin
void ei_app_invalidate_rect(const ei_rect_t* rect) {
    if(clipper_final == NULL) {
        clipper_final = malloc(sizeof(ei_rect_t));
    }
    ei_linked_rect_t* nouveau  = malloc(sizeof(ei_linked_rect_t));
    nouveau->rect = *rect;
    nouveau->next = NULL;
    int x_more_left = rect->top_left.x;
    int new_width =  rect->size.width;
    int y_more_top = rect->top_left.y;
    int new_height = rect->size.height;

    if (surfaces_mise_a_jour == NULL) {
        surfaces_mise_a_jour = nouveau;
        clipper_final = &((ei_rect_t){{x_more_left, y_more_top}, {new_width,new_height}});
    }
    else {
        int actual_x = clipper_final->top_left.x;
        int actual_y = clipper_final->top_left.y;
        int actual_width = clipper_final->size.width;
        int actual_height = clipper_final->size.height;

        int final_x =min(x_more_left, actual_x);
        int final_y =min(y_more_top, actual_y);
        int final_width = max(actual_width, new_width);
        int final_height = max(actual_height, new_height);

        clipper_final->top_left.x = final_x;
        clipper_final->top_left.y = final_y;
        clipper_final->size.width = final_width;
        clipper_final->size.height= final_height;
        ei_linked_rect_t* copie = surfaces_mise_a_jour;
        while(copie->next != NULL) {
            copie = copie->next;

        }
        copie->next = nouveau;
    }
}


// fonction min et max
int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a < b) ? b : a;
}

/* cette fonction est chargé de parcourir tout les rectangle pour mettre le gros clipper à jour
 * Mais c'est bon au lieu de ça je vais dire, à chaque fois que j'ajoute un rectangle,
 * je mets à jour les coordonnée du clipper final.
 */
//ei_rect_t* clipper_mise_à_jour(ei_linked_rect_t*) {}
