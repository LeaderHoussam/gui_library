//
// Created by traorefv on 29/04/24.
// modified by aharbilh on 07/05/24
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"

#include <ei_widget_configure.h>
#include "ei_widget.h"
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
uint32_t compteur_pick_id = 256;
bool top_toplevel = NULL;
bool btm_toplevel = NULL;
ei_point_t pt_init_toplevel = {0,0};


ei_surface_t offscreen = NULL;
//ei_event_t *event;

extern ei_widgetclass_t* liste_des_classe;
int min(int a, int b);
int max(int a, int b);

//ei_rect_t* clipper_mise_à_jour(ei_linked_rect_t*);
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
    ei_default_font  = hw_text_font_create	(ei_default_font_filename,ei_style_normal, ei_font_default_size);

    ei_widgetclass_t* classe_toplevel = init_toplevel_classe();
    ei_widgetclass_register(classe_toplevel);
    printf("j'ai reussi à ajouter la classe frame");

    // on initialise ici les gestionnaires de geometrie
    ei_geometrymanager_t* placeur = init_placeur();
    ei_geometrymanager_register (placeur);

    //enregistrement des geometry:

    root_window =   hw_create_window(main_window_size, fullscreen);
    offscreen = hw_surface_create(root_window, hw_surface_get_size(root_window), false);

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

bool do_nothing(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    return true;
}

bool search_binding(ei_event_t event, ei_widget_t){
}

void ei_app_run(void){
    //ei_bind(ei_ev_mouse_move, NULL, "all", do_nothing,NULL);
    ei_widget_t racine = ei_app_root_widget();
    const ei_surface_t *surface_principale = ei_app_root_surface();
    //offscreen = hw_surface_create(surface_principale, hw_surface_get_size(surface_principale), false);

    // Redessiner la racine avant d'entrer dans la boucle des événements
    (*(racine->wclass->drawfunc))(racine, surface_principale, offscreen, racine->content_rect);
    hw_surface_update_rects(surface_principale, NULL);
    surfaces_mise_a_jour = NULL;
    clipper_final = NULL;
    ei_event_t *event = calloc(1, sizeof(ei_event_t));
    //hw_event_post_app(EVENT_BINDINGS);
    ei_widget_t current_widget;

    while (!quit_requested) {
        // Attendre le prochain événement utilisateur
        //ei_eventtype_t event;

        hw_event_wait_next(event);
        //On parcours toute la liste des bindings, c'est pas trop optimisé mais on fait comme ça pour le moment
        ei_event_binding *event_bind;
        event_bind = EVENT_BINDINGS;
        if (event->type == ei_ev_exposed){
            continue;
        }
        if (event->type != ei_ev_mouse_move) {
            current_widget = ei_widget_pick(&(event->param.mouse.where));
            //while (event->type != event_bind->event_type && current_widget != event_bind->widget) { //
            while (!(event->type == event_bind->event_type && current_widget == event_bind->widget)){
                event_bind = event_bind->next;
                if (event_bind->next == NULL) {
                    break;
                }
            }
            event_bind->callback(current_widget, event, NULL);
            //current_widget->wclass->drawfunc(current_widget, );
            (*(racine->wclass->drawfunc))(racine, surface_principale, offscreen, racine->content_rect);
            hw_surface_update_rects(surface_principale, surfaces_mise_a_jour);
        } else if (event->type == ei_ev_mouse_move && (top_toplevel || btm_toplevel)){
            printf("\n hna \n");
            //current_widget = ei_widget_pick(&(event->param.mouse.where));
            while (!(event->type == event_bind->event_type && current_widget == event_bind->widget)){
                event_bind = event_bind->next;
                if (event_bind->next == NULL) {
                    break;
                }
            }
            event_bind->callback(current_widget, event, NULL);
            (*(racine->wclass->drawfunc))(racine, surface_principale, offscreen, racine->content_rect);
            hw_surface_update_rects(surface_principale, surfaces_mise_a_jour);
        }
    }
    //ei_unbind(ei_ev_mouse_move, NULL, "all", do_nothing,NULL);
    free(event);
    hw_surface_free(offscreen);
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
    nouveau->rect.size.width += 30;
    nouveau->rect.size.height += 30;
    nouveau->next = NULL;
    int x_more_left = rect->top_left.x;
    int new_width =  rect->size.width+30; // j'ajoute ces plus trente pour tenir compte du top level
    int y_more_top = rect->top_left.y;
    int new_height = rect->size.height+30;

    if (surfaces_mise_a_jour == NULL) {
        surfaces_mise_a_jour = nouveau;
        clipper_final->top_left = (ei_point_t){x_more_left, y_more_top};
        clipper_final->size = (ei_size_t) {new_width,new_height};
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
        clipper_final->size.width = final_width+30;
        clipper_final->size.height= final_height+30;
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
