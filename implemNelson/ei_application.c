//
// Created by traorefv on 29/04/24.
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"

#include <ei_widget_configure.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "ei_implementation.h"

ei_surface_t root_window = NULL;
ei_widget_t root_widget = NULL;
ei_linked_rect_t* surfaces_mise_a_jour = NULL;
ei_rect_t* clipper_final = NULL;
uint32_t compteur_pick_id = 1;


extern ei_widgetclass_t* liste_des_classe;
int min(int a, int b);
int max(int a, int b);
//ei_rect_t* clipper_mise_à_jour(ei_linked_rect_t*);
void ei_app_create(ei_size_t main_window_size, bool fullscreen){
    hw_init();

    //enregistrement des classes:
    // pour l'instant, on enregistre  juste la classe frame
    ei_default_font  = hw_text_font_create	(ei_default_font_filename,ei_style_normal, ei_font_default_size);
    // on initialise la liste des classes
    printf("j'ai bien fait le init");
    ei_widgetclass_t* classe_frame = init_frame_classe();
    ei_widgetclass_register(classe_frame);

    ei_widgetclass_t* classe_button = init_button_classe();
    ei_widgetclass_register(classe_button);

    ei_widgetclass_t* classe_toplevel = init_toplevel_classe();
    ei_widgetclass_register(classe_toplevel);


    // on initialise ici les gestionnaires de geometrie
    ei_geometrymanager_t* geo_placeur = init_placeur();
    ei_geometrymanager_register(geo_placeur);

    printf("j'ai reussi à ajouter la classe frame");

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

void ei_app_run(void){
    //
    ei_widget_t racine = ei_app_root_widget();
    const ei_surface_t surface_principale = ei_app_root_surface();
    ei_surface_t surface_arriere = hw_surface_create(surface_principale, hw_surface_get_size(surface_principale), false);
    // on doit normalement parcourir ici la hierarchie pour
    // afficher, mais je ne la comprend pas bien encore,
    // on affihce donc notre seul frame root  d'abord pour voir

    //ici on affiche tout l'écran initiale avant de rentrer dans la gestion des events
    (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, racine->content_rect);
    hw_surface_update_rects(surface_principale, NULL);
    /* on met le clipper de la racine à NULL,
     * ensuite on le modifiera pour le faire pointer vers nos surfaces mises à jour
     * (on va créer une variable globale pour gèrer les surfaces mises à jour)
     * ei_rect_t* surfaces_mis_a_jour;
     */

    //on sera dans la boucle ici qui va recevoir les évènements
    /* du style: while(event) : if surfaces_mis_a_jour != NULL: redessiner sinon attendre un autre event
     * on appelera le dessin du root avec clipper = surfaces_mises_a_jours et pareil pour
     * hw_surfaces updates_rect.
     */

    hw_surface_free(surface_arriere);
    getchar();
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
