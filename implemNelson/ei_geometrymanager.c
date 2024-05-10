//
// Created by aharbilh on 06 May 2024.
//

#include <stdlib.h>
#include <stdio.h>
#include "ei_geometrymanager.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_implementation.h"

ei_geometrymanager_t* liste_de_gestionnaires = NULL;

size_t		ei_geom_param_size();

ei_geometrymanager_runfunc_t geomrunfunc(ei_widget_t widget){
    widget->screen_location.top_left.x;
}

void ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location) {

    // si y'a eu un changement de la géometry:
    if (new_screen_location->top_left.x != widget->screen_location.top_left.x ||
        new_screen_location->top_left.y != widget->screen_location.top_left.y ||
        new_screen_location->size.width != widget->screen_location.size.width ||
        new_screen_location->size.height != widget->screen_location.size.height) {

        //schedule a redraw of the screen on the old and new screen location
        ei_app_invalidate_rect(&(widget->screen_location));
        ei_app_invalidate_rect(new_screen_location);

        // notify the widget that it's geometry has changed
        // on a modifier les dimensions et positions du widget
        widget->screen_location = *new_screen_location;
        widget->wclass->geomnotifyfunc(widget);

        // recompute the geometry of the children

        /* je me demande, que se passe t'il si le fils n'a pas de gestionnaire de geometrie
         AH c'est bon: il n'est pas dessiné
         */
        if (widget->children_head != NULL) {
            widget->children_head->geom_params->manager->runfunc(widget->children_head);
        }

        if (widget->next_sibling != NULL) {
            widget->next_sibling->geom_params->manager->runfunc(widget->next_sibling);
        }
    }
}

// cette fonction verifie qu'on ne place pas le point or du parent
void set_value(int *x, int *y, ei_point_t pos_parent, ei_size_t dim_parent) {
    if ((*x < pos_parent.x) || (*x > pos_parent.x + dim_parent.width)) {
        *x = pos_parent.x;
    }
    if ((*y < pos_parent.y) || (*y > pos_parent.y + dim_parent.height)) {
        *y = pos_parent.x;
    }
}

void placeur_runfunc(ei_widget_t widget) {
    // les paramètre du parent:
    ei_widget_t parent = widget->parent;
    ei_point_t pos_parent = parent->screen_location.top_left;
    ei_size_t dim_parent = parent->screen_location.size;
    //ei_rect_t* content_rect_parent = parent->content_rect;
    //ei_rect_ptr_t* content_rect_parent = &(widget->parent->content_rect);

    // les paramètres de config  du widget (et sa localisation initiale)
    ei_impl_placeur_t *geo_widget = (ei_impl_placeur_t *) widget->geom_params;
    ei_point_t *pos_widget = &(widget->screen_location.top_left);
    ei_size_t *dim_widget = &(widget->screen_location.size);

    // il faut maintenant faire plein de mathématiques. AH AH AH!


    // gestion de la largeur et de la hauteur finales
    int final_width = geo_widget->width + geo_widget->rel_width * dim_parent.width;
    /*if (final_width < 0 || final_width > dim_parent.width) {
        final_width = dim_parent.width;
    }*/
    int final_height = geo_widget->height + geo_widget->rel_width * dim_parent.height;
    /*if (final_height < 0 || final_height > dim_parent.height) {
        final_height = dim_parent.width;
    }*/

    // maintenant on s'attaque au (x, y) final du widget;
    ei_anchor_t anchor = geo_widget->anchor;
    //ici je me demande si x, et y sont relatif au parent. oui!
    int x_init = pos_parent.x + geo_widget->x + geo_widget->rel_x * dim_parent.width;
    int y_init = pos_parent.y + geo_widget->y + geo_widget->rel_y * dim_parent.height;
    int x_final = 0;
    int y_final = 0;

    switch (anchor) {
        case ei_anc_northwest:
            x_final = x_init;
            y_final = y_init;
            //set_value(&x_final, &y_final, pos_parent, dim_parent);
            break;
        case ei_anc_center:
            x_final = x_init - final_width / 2;
            y_final = y_init - final_height / 2;
            break;
        case ei_anc_east:
            x_final = x_init - final_width;
            y_final = y_init - final_height / 2;
            break;
        case ei_anc_west:
            x_final = x_init;
            y_final = y_init - final_height / 2;
            break;
        case ei_anc_north:
            x_final = x_init - final_width / 2;
            y_final = y_init;
            break;
        case ei_anc_south:
            x_final = x_init - final_width / 2;
            y_final = y_init - final_height;
            break;
        case ei_anc_northeast:
            x_final = x_init - final_width;
            y_final = y_init;
            break;
        case ei_anc_southeast:
            x_final = x_init - final_width;
            y_final = y_init - final_height;
            break;
        case ei_anc_southwest:
            x_final = x_init;
            y_final = y_init - final_height;
            break;
        case ei_anc_none:
            x_final = x_init;
            y_final = y_init;
            break;
            // il faut gerer les cas ou la widget sortent du root_widget
        default:
            printf("On ne devrait jamais en arriver là. AH AH AH");
    }
    ei_rect_t *nouveaux_affichage = &((ei_rect_t) {(ei_point_t) {x_final, y_final},
                                                   (ei_size_t) {final_width, final_height}});
// je ne sais pas s'il faut allouer ici ou pas
   // ei_rect_t *nouveaux_affichage = malloc(sizeof(ei_rect_t));
    //ouveaux_affichage = &((ei_rect_t) {(ei_point_t) {x_final, y_final}, (ei_size_t) {final_width, final_height}});

    /*il faudra commenter les deux lignes suivantes je crois pour bien comprendre encore*/
    //widget->screen_location.size = (ei_size_t) {final_width, final_height};
    //widget->screen_location.top_left = (ei_point_t) {x_final, y_final};

    ei_geometry_run_finalize(widget, nouveaux_affichage);
}

void placeur_releasefunc( ei_widget_t widget) {
}

//création du placeur : gestionaire de géométrie
ei_geometrymanager_t* init_placeur(void){
    // Créer et initialiser une structure ei_geometrymanager_t pour le placeur
    ei_geometrymanager_t* placer_manager = calloc(1,sizeof(ei_geometrymanager_t));
    // Remplir les informations du gestionnaire de géométrie "placeur"
    strcpy(placer_manager->name, "placeur");
    placer_manager->runfunc = &(placeur_runfunc);
    placer_manager->releasefunc = &(placeur_releasefunc);
    placer_manager->next = NULL;
    return placer_manager;
}

void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
    ei_geometrymanager_t* tete = liste_de_gestionnaires;
    if (tete==NULL){
        liste_de_gestionnaires = geometrymanager;
    } else{
        while (tete->next != NULL) {
            tete = tete->next;
        }
        tete->next = geometrymanager;
    }
}

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name){
    ei_geometrymanager_t* tete = liste_de_gestionnaires;
    while(tete != NULL) {
        // ici, on compare le nom de la classe en tête de liste avec le nom de frame rechercher
        if (strcmp(name, tete->name) == 0) {
            return tete;
        }
        tete = tete->next;
    }
    return NULL;
}

ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget){
    return widget->geom_params->manager;
}

ei_geom_param_t		ei_widget_get_geom_params	(ei_widget_t widget){
    return widget->geom_params;
}

void			ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager){

}

void			ei_widget_set_geom_params	(ei_widget_t widget, ei_geom_param_t geom_param){

}

void			ei_geometrymanager_unmap	(ei_widget_t widget){
    // Vérifie si le widget est géré par un gestionnaire de géométrie
    ei_geometrymanager_t* manager = ei_widget_get_geom_manager(widget);
    if (manager == NULL) {
        // Si le widget n'est pas géré, la fonction retourne silencieusement
        return;
    }

    // Appel de la fonction de libération du gestionnaire de géométrie
    if (manager->releasefunc != NULL) {
        manager->releasefunc(widget);
    }

    // Libération des paramètres de géométrie du widget
    ei_geom_param_t geom_param = ei_widget_get_geom_params(widget);
    if (geom_param != NULL) {
        free(geom_param);
        // Réinitialisation des paramètres de géométrie du widget à NULL
        ei_widget_set_geom_params(widget, NULL);
    }

    // Invalidation de la position actuelle du widget pour déclencher un redessin
    widget->screen_location.top_left.x = 0;
    widget->screen_location.top_left.y = 0;
    widget->screen_location.size.width = 0;
    widget->screen_location.size.height = 0;

    // Réinitialisation de la position du widget à 0
    widget->screen_location.top_left.x = 0;
    widget->screen_location.top_left.y = 0;
}
