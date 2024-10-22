//
// Created by traorefv on 30/04/24.
//

#include "ei_widget.h"

#include <ei_application.h>

#include "ei_implementation.h"
#include "ei_widgetclass.h"

link_widget* liste_des_widgets = NULL;
// dans cette fonction il faudra revoir comment bien
// chainer les widgets
// et aussi comment faire pour refuser la création d'un widget de parent NULL, sauf la racine
ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                            ei_widget_t		parent,
                                            ei_user_param_t	user_data,
                                            ei_widget_destructor_t destructor){

    /*if (parent == NULL & parent != root_widget) {
        return NULL;
    }*/

    // la classe du widget à creer; on suppose ici qu'elle existe
    ei_widgetclass_t* classe_du_widget = ei_widgetclass_from_name(class_name);
    // creation d'une instance de widget:

    ei_widget_t new_widget = classe_du_widget->allocfunc();
    new_widget->wclass = classe_du_widget;

    //ce bloc est à revoir
    new_widget->pick_id = compteur_pick_id;
    new_widget->pick_color = map_pick_id_to_color(surface_arriere, compteur_pick_id);
    new_widget->user_data = NULL;
    compteur_pick_id += 256;
    // ici, si le parent est NULL

    new_widget->parent = parent;
    //Widget Hierachy Management *//*
    // il faut que je revois le chai voir le chaînage , mais à quel niveau?
    new_widget->children_head = NULL;
    new_widget->children_tail = NULL;
    new_widget->next_sibling = NULL;



    if (parent != NULL) {
        if (parent->children_head == NULL) {
            parent->children_head = new_widget;
            parent->children_tail = new_widget;
        }
        /*
        if(parent->children_tail != NULL) {
            parent->children_tail->next_sibling = new_widget;
        }
        new_widget->next_sibling = NULL;
        parent->children_tail = new_widget;
    */
    else {
        // Ajouter à la fin
        parent->children_tail->next_sibling = new_widget;
        parent->children_tail = new_widget;
    }

    }
    new_widget->destructor = destructor;
    new_widget->user_data = user_data;

    // Geometry Management *//*
    // je dois revenir sur cette fonction pour la comprendre et l'initialiser sinon rien ne s'affiche à l'écran
    new_widget->geom_params = NULL;

   /* if (new_widget->parent != NULL) {
        // on initialise les dimentions à  0
        new_widget->requested_size = (ei_size_t){0,0};
    }*/

    new_widget->requested_size = (ei_size_t){0,0};
    new_widget->screen_location.size = (ei_size_t){0,0};

    new_widget->screen_location.top_left.x = 0;
    new_widget->screen_location.top_left.y = 0;
    new_widget->content_rect = &(new_widget->screen_location);


    classe_du_widget->setdefaultsfunc(new_widget);


    //à revoir pour gerer le cas ou il s'agit de la racine'
    //parent->children_head = new_widget;


    // il faudra ici bien faire les liaisons entres les fils et fréres de widget
    // j'y reviendrai
    //new_widget->next_sibling =
    /*
    link_widget* new = malloc(sizeof(link_widget));
    new->widget = new_widget;
    new->next = NULL;
    if (liste_des_widgets==NULL) {

        liste_des_widgets =  new;
    }
    else {
        link_widget* tete = liste_des_widgets;
        while (tete->next != NULL) {
            tete = tete->next;
        }
        tete->next = new;
    }

    */

    return new_widget;

}


void			ei_widget_destroy		(ei_widget_t		widget) {
    // à modifier
    //widget->geom_params = NULL;
    //widget->wclass->releasefunc(widget);
    delete_widget(widget);
    ei_app_invalidate_rect(&widget->screen_location);
}

bool	 		ei_widget_is_displayed		(ei_widget_t		widget) {
    return (widget->geom_params != NULL);
}

bool color_equal(ei_color_t *widget_color, ei_color_t pixel_color){
    return (widget_color->red == pixel_color.red &&
            widget_color->green == pixel_color.green &&
            widget_color->blue == pixel_color.blue);
}

/*
ei_widget_t pick_recursive(ei_widget_t widget, ei_color_t* color) {
    if (widget == NULL){
        return NULL;
    }
    if (color_equal(widget->pick_color, *color)){
        return widget;
    } else {
        //if (widget != root_widget){
        while(widget->next_sibling != NULL){
            return pick_recursive(widget->next_sibling, color);
        }
        //} else
        return pick_recursive(widget->children_head, color);
    }
}
*/

ei_widget_t pick_recursive(ei_widget_t widget, ei_color_t* color) {
    if (widget == NULL) {
        return NULL;
    }

    // Vérifier si le widget actuel a la couleur de piquage recherchée
    if (color_equal(widget->pick_color, *color)) {
        return widget;
    }

    // Parcourir les enfants du widget actuel
    ei_widget_t result = pick_recursive(widget->children_head, color);
    if (result != NULL) {
        return result;
    }

    // Parcourir les frères et sœurs du widget actuel
    return pick_recursive(widget->next_sibling, color);
}