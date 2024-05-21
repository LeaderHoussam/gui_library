//
// Created by traorefv on 29/04/24.
// modified by aharbilh on 07/05/24
//
//
// Created by traorefv on 29/04/24.
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"

#include <ei_utils.h>
#include <ei_widget_configure.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_placer.h"

ei_surface_t root_window = NULL;
ei_surface_t offscreen = NULL;
ei_widget_t root_widget = NULL;
ei_linked_rect_t* surfaces_mise_a_jour = NULL;
ei_rect_t* clipper_final = NULL;
uint32_t compteur_pick_id = 256;//16384;
ei_point_t pos_mouse = (ei_point_t){0,0};

bool quitter = false;
//ei_font_t ei_default_font = NULL;

//extern ei_widgetclass_t* liste_des_classe;

int min(int a, int b);
int max(int a, int b);
//ei_rect_t* clipper_mise_à_jour(ei_linked_rect_t*);
void ei_app_create(ei_size_t main_window_size, bool fullscreen){
    hw_init();
    // on initialise la liste des classes
    printf("j'ai bien fait le init");
    ei_widgetclass_t* classe_frame = init_frame_classe();
    ei_widgetclass_register(classe_frame);

    ei_widgetclass_t* classe_button = init_button_classe();
    ei_widgetclass_register(classe_button);

    ei_widgetclass_t* classe_toplevel = init_toplevel_classe();
    ei_widgetclass_register(classe_toplevel);

    ei_widgetclass_t* classe_entry = init_entry_classe();
    ei_widgetclass_register(classe_entry);

    // on initialise ici les gestionnaires de geometrie
    ei_geometrymanager_t* geo_placeur = init_placeur();
    ei_geometrymanager_register(geo_placeur);

    printf("j'ai reussi à ajouter la classe frame");

    curseur  = calloc(2, sizeof(ei_point_t));
    //enregistrement des geometry:

    root_window =   hw_create_window(main_window_size, fullscreen);
    offscreen = hw_surface_create(root_window, hw_surface_get_size(root_window), false);
    //surface_arriere = hw_create_window(main_window_size, fullscreen);
    //hw_surface_lock(root_window);
    root_widget = ei_widget_create("frame", NULL,NULL, NULL);
    ei_size_t taille = hw_surface_get_size(root_window);
    ei_frame_configure(root_widget, &taille, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    //hw_surface_unlock(root_window);

    ei_bind(ei_ev_mouse_buttondown, NULL, "button",bouton_handler, NULL);
    ei_bind(ei_ev_mouse_buttonup, NULL, "button",bouton_handler, NULL);
    ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel",toplevel_handler, NULL);
    ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel",toplevel_handler_1, NULL);
    ei_bind(ei_ev_mouse_buttondown,NULL,"entry", entry_handler,NULL);
}

ei_widget_t ei_app_root_widget(void) {
    return root_widget;

}

void ei_app_run(void){
    //
    ei_widget_t racine = ei_app_root_widget();
    const ei_surface_t surface_principale = ei_app_root_surface();


    (*(racine->wclass->drawfunc))(racine, surface_principale, offscreen, racine->content_rect);
    hw_surface_update_rects(surface_principale, NULL);

    liberer_ei_rect(&clipper_final);
    liberer_ei_linked_rect(&surfaces_mise_a_jour);
    ei_event_t* evenement = calloc(1,sizeof(ei_event_t));


    while(!quitter) {

        if (surfaces_mise_a_jour != NULL) {
            (*(racine->wclass->drawfunc))(racine, surface_principale, offscreen,clipper_final);
            hw_surface_update_rects(surface_principale,surfaces_mise_a_jour);
            liberer_ei_linked_rect(&surfaces_mise_a_jour);
            liberer_ei_rect(&clipper_final);
        }
        hw_event_wait_next(evenement);
        ei_eventtype_t type_event = evenement->type;
        traitant_t* liste = trouve_traitant(type_event);

        while( liste != NULL && !execute_traitant(evenement, *liste)) {
            liste = liste->next;
        }
    }
    free(evenement);
    hw_surface_free(offscreen);
}

void ei_app_free(void){
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", bouton_handler_1, NULL);
    ei_unbind(ei_ev_mouse_move, NULL, "all", bouton_handler_1, NULL);
    hw_quit();
}

ei_surface_t ei_app_root_surface(void) {
    return root_window;
}

// permet de quitter l'application
void ei_app_quit_request(void) {
    quitter = true;
}

// chaque fois qu'on rentre dans cette fonction
// on actualise le clipper final ou refaire nos dessin

void ei_app_invalidate_rect(const ei_rect_t* rect) {
    if(clipper_final == NULL) {
        clipper_final = malloc(sizeof(ei_rect_t));
    }
    ei_linked_rect_t* nouveau  = malloc(sizeof(ei_linked_rect_t));
    nouveau->rect = *rect;
    nouveau->rect.size.width += 30;
    nouveau->rect.size.height += 30;
    nouveau->next = NULL;

    ei_rect_t* new = trouve_inter_rect(nouveau->rect, root_widget->screen_location);
    ei_rect_t* final;
    if (surfaces_mise_a_jour == NULL) {

        nouveau->rect = *new;
        surfaces_mise_a_jour = nouveau;
        *clipper_final = nouveau->rect;
        final = trouve_inter_rect(root_widget->screen_location, *clipper_final);
        *clipper_final = *final;
    }
    else {

        ei_rect_t* clip = trouve_rect_contenant(*clipper_final, nouveau->rect);
        final = trouve_inter_rect(root_widget->screen_location, *clip);
        clipper_final->top_left = final->top_left;
        clipper_final->size = final->size;
        free(clip);

        nouveau->rect = *new;
        nouveau->next = surfaces_mise_a_jour;
        surfaces_mise_a_jour = nouveau;
    }
    free(new);
    free(final);
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

void liberer_ei_linked_rect(ei_linked_rect_t** liste) {
    ei_linked_rect_t* current = *liste;
    while (current != NULL) {
        ei_linked_rect_t* next = current->next;
        free(current);
        current = next;
    }
    *liste = NULL;
}

void liberer_ei_rect(ei_rect_t** clip) {
    ei_rect_t* current = *clip;
    free(current);
    *clip = NULL;
}



ei_rect_t* trouve_rect_contenant(ei_rect_t rec1, ei_rect_t rec2) {
    ei_point_t coin_gauche;
    ei_size_t dim;
    ei_rect_t* retour = malloc(sizeof(ei_rect_t));

    // Trouver les coordonnées du coin supérieur gauche
    coin_gauche.x = (rec1.top_left.x < rec2.top_left.x) ? rec1.top_left.x : rec2.top_left.x;
    coin_gauche.y = (rec1.top_left.y < rec2.top_left.y) ? rec1.top_left.y : rec2.top_left.y;

    retour->top_left = coin_gauche;
    // les coordonnes en bas à droite
    int droit1 = rec1.top_left.x + rec1.size.width;
    int droit2 = rec2.top_left.x + rec2.size.width;
    int bas1 = rec1.top_left.y + rec1.size.height;
    int bas2 = rec2.top_left.y + rec1.size.height;

    dim.width =  (droit1 > droit2) ? (droit1 - coin_gauche.x) : (droit2 - coin_gauche.x);
    dim.height = (bas1 > bas2) ? (bas1 - coin_gauche.y) : (bas2 - coin_gauche.y);

    retour->size = dim;

    return retour;
}



ei_rect_t* trouve_inter_rect(ei_rect_t rect1, ei_rect_t rect2) {
    ei_rect_t* retour = malloc(sizeof(ei_rect_t));
    ei_rect_t result;

    // Calcul des coordonnées du coin supérieur gauche du rectangle d'intersection

    result.top_left.x = (rect1.top_left.x > rect2.top_left.x) ? rect1.top_left.x : rect2.top_left.x;
    result.top_left.y = (rect1.top_left.y > rect2.top_left.y) ? rect1.top_left.y : rect2.top_left.y;
    // Calcul des coordonnées du coin inférieur droit du rectangle d'intersection
    int right1 = rect1.top_left.x + rect1.size.width;
    int right2 = rect2.top_left.x + rect2.size.width;
    int bottom1 = rect1.top_left.y + rect1.size.height;
    int bottom2 = rect2.top_left.y + rect2.size.height;
    result.size.width = (right1 < right2) ? (right1 - result.top_left.x) : (right2 - result.top_left.x);
    result.size.height = (bottom1 < bottom2) ? (bottom1 - result.top_left.y) : (bottom2 - result.top_left.y);

    // Si les rectangles ne se chevauchent pas, le rectangle d'intersection est vide
    if (result.size.width < 0 || result.size.height < 0) {
        result.top_left.x = result.top_left.y = result.size.width = result.size.height = 0;
    }

    *retour = result;
    return retour;
}
