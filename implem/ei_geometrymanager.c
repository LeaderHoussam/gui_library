//
// Created by ah aharbilh on 06 May 2024.
//

#include "ei_geometrymanager.h"

#include "ei_types.h"
#include "ei_widget.h"
#include "ei_implementation.h"

struct ei_impl_geom_param_t;
typedef struct ei_impl_geom_param_t*	ei_geom_param_t;


size_t		ei_geom_param_size();

typedef char	ei_geometrymanager_name_t[20];

typedef void	(*ei_geometrymanager_runfunc_t)		(ei_widget_t widget);

typedef void	(*ei_geometrymanager_releasefunc_t)	(ei_widget_t widget);

ei_geometrymanager_runfunc_t geomrunfunc(ei_widget_t widget){
    widget->screen_location.top_left.x
}

//création du placeur : gestionaire de géométrie
ei_geometrymanager_t* init_placeur(){
    // Créer et initialiser une structure ei_geometrymanager_t pour le placeur
    ei_geometrymanager_t* placer_manager = calloc(1,sizeof(ei_geometrymanager_t));
    // Remplir les informations du gestionnaire de géométrie "placeur"
    strcpy(placer_manager->name, "placeur");
    placer_manager->runfunc = NULL;
    placer_manager->releasefunc = ei_geometrymanager_unmap;
    placer_manager->next = NULL;
}

void			ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location);

ei_geometrymanager_t* liste_de_gestionnaire = NULL;

void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
    ei_geometrymanager_t* tete = liste_de_gestionnaire;
    if (tete==NULL){
        liste_de_gestionnaire = geometrymanager;
    } else{
        while (tete->next != NULL) {
            tete = tete->next;
        }
        tete->next = geometrymanager;
    }
}

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name);

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
