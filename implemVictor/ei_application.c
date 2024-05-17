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
ei_surface_t surface_arriere = NULL;
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
    //ei_default_font = hw_text_font_create(ei_default_font_filename,ei_style_normal,ei_font_default_size);
    //enregistrement des classes:
    // pour l'instant, on enregistre  juste la classe frame

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
    surface_arriere = hw_surface_create(root_window, hw_surface_get_size(root_window), false);
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



}

ei_widget_t ei_app_root_widget(void) {
    return root_widget;

}

void ei_app_run(void){
    //
    ei_widget_t racine = ei_app_root_widget();
    const ei_surface_t surface_principale = ei_app_root_surface();
    //surface_arriere = hw_surface_create(surface_principale, hw_surface_get_size(surface_principale), false);

    // on doit normalement parcourir ici la hierarchie pour
    // afficher, mais je ne la comprend pas bien encore,
    // on affihce donc notre seul frame root  d'abord pour voir



    //ici on affiche tout l'écran initiale avant de rentrer dans la gestion des events

/*
    (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, racine->content_rect);
    hw_surface_update_rects(surface_principale, NULL);

/*

    /* on met le clipper de la racine à NULL,
     * ensuite on le modifiera pour le faire pointer vers nos surfaces mises à jour
     * (on va créer une variable globale pour gèrer les surfaces mises à jour)
     * ei_rect_t_linked_rect* surfaces_mis_a_jour;
     */
    /*free(surfaces_mise_a_jour);
    free(clipper_final);
    surfaces_mise_a_jour = NULL;
    clipper_final = NULL;*/


    //on sera dans la boucle ici qui va recevoir les évènements
    /* du style: while(event) : if surfaces_mis_a_jour != NULL: redessiner sinon attendre un autre event
     * on appelera le dessin du root avec clipper = surfaces_mises_a_jours et pareil pour
     * hw_surfaces updates_rect.
     */



    (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, racine->content_rect);
    hw_surface_update_rects(surface_principale, NULL);

    //ei_place_xy(racine->children_head, 50,40);
    //hw_surface_update_rects(surface_arriere, NULL);

    /*
    ei_color_t* col_pic = get_pick_screen_color((ei_point_t){350, 250});
    printf("la couleur de la pickcolor  à 350, 250 est : \n"
           "r : %u, g: %u, b: %u, a: %u ", col_pic->red, col_pic->green, col_pic->blue, col_pic->alpha);
*/
    //surfaces_mise_a_jour = NULL;
    //clipper_final = NULL;
    liberer_ei_rect(&clipper_final);
    liberer_ei_linked_rect(&surfaces_mise_a_jour);
    ei_event_t* evenement = calloc(1,sizeof(ei_event_t));


    while(!quitter) {

        if (surfaces_mise_a_jour != NULL) {

           // (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere, racine->content_rect);
            //hw_surface_update_rects(surface_principale, surfaces_mise_a_jour);


            (*(racine->wclass->drawfunc))(racine, surface_principale, surface_arriere,clipper_final);
            hw_surface_update_rects(surface_principale,surfaces_mise_a_jour);
            liberer_ei_linked_rect(&surfaces_mise_a_jour);
            liberer_ei_rect(&clipper_final);
        }



        //free(surfaces_mise_a_jour);
        //clipper_final->size = (ei_size_t){0,0};
        //clipper_final->top_left = (ei_point_t){0,0};


        //ei_app_quit_request(); // à commenter quand tout sera bon
        hw_event_wait_next(evenement);
        //pos_mouse = evenement->param.mouse.where;
        // il faut maintenant traiter l'évènement
        //event_with_callback* mes_types_event = liste_des_events_enregistres;
        ei_eventtype_t type_event = evenement->type;
        traitant_t* liste = trouve_traitant(type_event);
        //traitant_t* liste = type_evenement->liste_des_traitants;
        while( liste != NULL && !execute_traitant(evenement, *liste)) {

                liste = liste->next;
        }



    }

    //hw_surface_free(surface_arriere);
    //getchar();


}

void ei_app_free(void){
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



    if (surfaces_mise_a_jour == NULL) {
        surfaces_mise_a_jour = nouveau;
       // int x_more_left = rect->top_left.x;
        //int new_width =  rect->size.width+30; // j'ajoute ces plus trente pour tenir compte du top level
        //int y_more_top = rect->top_left.y;
        //int new_height = rect->size.height+30;

        //clipper_final->top_left = (ei_point_t){x_more_left, y_more_top};
        //clipper_final->size = (ei_size_t) {new_width,new_height};
        *clipper_final = nouveau->rect;
    }
    else {

        ei_rect_t* clip = trouve_rect_contenant(*clipper_final, nouveau->rect);
        ei_rect_t* final = trouve_inter_rect(root_widget->screen_location, *clip);
        clipper_final->top_left = final->top_left;
        clipper_final->size = final->size;
        free(clip);
        free(final);

        /*
        ei_rect_t* surf_final = trouve_rect_contenant(surfaces_mise_a_jour->rect, nouveau->rect);
        surfaces_mise_a_jour->rect = *surf_final;
        */
       // ei_linked_rect_t* copie = surfaces_mise_a_jour;
        /*
        while(copie->next != NULL) {
            copie = copie->next;

        }*/

        //ei_linked_rect_t* copie = surfaces_mise_a_jour;
        ei_rect_t* fin = trouve_inter_rect(root_widget->screen_location, nouveau->rect);
        nouveau->rect = *fin;
        nouveau->next = surfaces_mise_a_jour;
        surfaces_mise_a_jour = nouveau;
        free(fin);
        //copie->next = nouveau;

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



/*
ei_rect_t* trouve_rect_contenant(ei_rect_t rect1, ei_rect_t rect2) {
    // Allocation dynamique du rectangle d'intersection
    ei_rect_t* intersection_rect = malloc(sizeof(ei_rect_t));
    if (intersection_rect == NULL) {
        // Gestion de l'échec de l'allocation mémoire
        return NULL;
    }

    // Calcul des coordonnées du rectangle d'intersection
    int x1 = rect1.top_left.x;
    int y1 = rect1.top_left.y;
    int x2 = rect1.top_left.x + rect1.size.width;
    int y2 = rect1.top_left.y + rect1.size.height;
    int x3 = rect2.top_left.x;
    int y3 = rect2.top_left.y;
    int x4 = rect2.top_left.x + rect2.size.width;
    int y4 = rect2.top_left.y + rect2.size.height;

    // Trouver les limites du rectangle d'intersection
    int x_left = max(x1, x3);
    int y_top = max(y1, y3);
    int x_right = min(x2, x4);
    int y_bottom = min(y2, y4);

    // Calculer la largeur et la hauteur du rectangle d'intersection
    int width = max(0, x_right - x_left);
    int height = max(0, y_bottom - y_top);

    // Réajuster les coordonnées si elles sont négatives
    if (x_left < 0) {
        width -= abs(x_left);
        x_left = 0;
    }
    if (y_top < 0) {
        height -= abs(y_top);
        y_top = 0;
    }

    // Déterminer si les coordonnées du coin supérieur gauche doivent être ajustées
    if (x_left < 0) {
        x_left = 0;
    }
    if (y_top < 0) {
        y_top = 0;
    }

    // Remplir le rectangle d'intersection
    intersection_rect->top_left.x = x_left;
    intersection_rect->top_left.y = y_top;
    intersection_rect->size.width = width;
    intersection_rect->size.height = height;

    // Retourner le pointeur vers le rectangle d'intersection
    return intersection_rect;
}
*/
/*
ei_rect_t* trouve_inter_rect(ei_rect_t rect1, ei_rect_t rect2) {
    // Allocation dynamique du rectangle d'intersection
    ei_rect_t* intersection_rect = malloc(sizeof(ei_rect_t));
    if (intersection_rect == NULL) {
        // Gestion de l'échec de l'allocation mémoire
        return NULL;
    }

    // Calcul des coordonnées du rectangle d'intersection
    int x1 = rect1.top_left.x;
    int y1 = rect1.top_left.y;
    int x2 = rect1.top_left.x + rect1.size.width;
    int y2 = rect1.top_left.y + rect1.size.height;
    int x3 = rect2.top_left.x;
    int y3 = rect2.top_left.y;
    int x4 = rect2.top_left.x + rect2.size.width;
    int y4 = rect2.top_left.y + rect2.size.height;

    // Trouver les limites du rectangle d'intersection
    int x_left = max(x1, x3);
    int y_top = max(y1, y3);
    int x_right = min(x2, x4);
    int y_bottom = min(y2, y4);

    // Calculer la largeur et la hauteur du rectangle d'intersection
    int width = max(0, x_right - x_left);
    int height = max(0, y_bottom - y_top);

    // Réajuster les coordonnées si elles sont négatives
    if (x_left < 0) {
        width -= abs(x_left);
        x_left = 0;
    }
    if (y_top < 0) {
        height -= abs(y_top);
        y_top = 0;
    }

    // Remplir le rectangle d'intersection
    intersection_rect->top_left.x = x_left;
    intersection_rect->top_left.y = y_top;
    intersection_rect->size.width = width;
    intersection_rect->size.height = height;

    // Retourner le pointeur vers le rectangle d'intersection
    return intersection_rect;
}
*/