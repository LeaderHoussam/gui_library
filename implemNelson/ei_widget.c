//
// Created by traorefv on 30/04/24.
//

#include "ei_widget.h"
#include "ei_implementation.h"
#include "ei_widgetclass.h"


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
    new_widget->pick_id = 0;
    new_widget->pick_color = NULL;
    new_widget->user_data = NULL;

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
        if(parent->children_tail != NULL) {
            parent->children_tail->next_sibling = new_widget;
        }
        new_widget->next_sibling = NULL;
        parent->children_tail = new_widget;

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


    return new_widget;
}

// à revoir toute la partie au-dessous

//nous devons parcourir la hiérarchie des widgets à partir de la racine,
// et pour chaque widget, vérifier si le point donné est à l'intérieur de sa zone de dessin.

/*
 * Parcourir récursivement la hiérarchie des widgets, commençant par la racine de l'application
 * et pour chaque widget, vérifie si le point donné est à l'intérieur de sa zone de dessin.
 * Si c'est le cas, elle vérifie également si le point est à l'intérieur des zones de dessin
 * de ses enfants. La fonction retourne le widget le plus haut dans la hiérarchie qui correspond
 * au point donné.
 * Si aucun widget ne correspond, elle retourne NULL.
 */

/*
ei_widget_t pick_recursive(ei_widget_t widget, ei_point_t *where) {
    // Vérifier si le point est à l'intérieur de la zone de dessin du widget
    if (ei_rect_contains(widget->screen_location, *where)) {
        // Si le widget a des enfants, vérifier s'il y a un widget enfant qui correspond également au point
        if (widget->children_head != NULL) {
            // Parcourir tous les enfants, du plus récent au plus ancien, pour respecter l'ordre d'affichage
            ei_widget_t enfant = widget->children_tail;
            while (enfant != NULL) {
                // Appeler récursivement cette fonction pour vérifier le widget enfant
                ei_widget_t widget_touche = pick_recursive(enfant, where);
                if (widget_touche != NULL) {
                    // Si un widget enfant correspond, le retourner
                    return widget_touche;
                }
                // Passer à l'enfant précédent
                enfant = enfant->prev_sibling;
            }
        }
        // Si aucun enfant ne correspond et que ce widget est touché, le retourner
        return widget;
    }
    // Si le point n'est pas à l'intérieur de la zone de dessin de ce widget, retourner NULL
    return NULL;
}

ei_widget_t		ei_widget_pick			(ei_point_t*		where){
    // Commencer à partir de la racine de l'application
    ei_widget_t racine = root_widget;
    // Appeler une fonction récursive pour parcourir les widgets et choisir celui qui correspond au point donné
    return pick_recursive(racine, where);
}*/

bool is_point_inside_widget(ei_widget_t widget, ei_point_t *where) {
    // Vérifier si le point est à l'intérieur de la zone de dessin du widget
    return (where->x >= widget->screen_location.top_left.x &&
            where->x < widget->screen_location.top_left.x + widget->screen_location.size.width &&
            where->y >= widget->screen_location.top_left.y &&
            where->y < widget->screen_location.top_left.y + widget->screen_location.size.height &&
            widget->pick_color != NULL);
}

ei_widget_t pick_recursive(ei_widget_t widget, ei_point_t *where) {
    // Vérifier si le point est à l'intérieur de la zone de dessin du widget
    if (is_point_inside_widget(widget, where)) {
        // Si le widget a des enfants, vérifier s'il y a un widget enfant qui correspond également au point
        if (widget->children_head != NULL) {
            // Parcourir tous les enfants, du plus récent au plus ancien, pour respecter l'ordre d'affichage
            ei_widget_t enfant = widget->children_head;
            while (enfant != NULL) {
                // Appeler récursivement cette fonction pour vérifier le widget enfant
                ei_widget_t widget_touche = pick_recursive(enfant, where);
                if (widget_touche != NULL) {
                    // Si un widget enfant correspond, le retourner
                    return widget_touche;
                }
                // Passer à l'enfant suivant
                enfant = enfant->next_sibling;
                // à revoir !
            }
        }
        // Si aucun enfant ne correspond et que ce widget est touché, le retourner
        return widget;
    }
    // Si le point n'est pas à l'intérieur de la zone de dessin de ce widget, retourner NULL
    return NULL;
}

ei_widget_t ei_widget_pick(ei_point_t *where) {
    // Commencer à partir de la racine de l'application
    ei_widget_t racine = root_widget;

    // Appeler une fonction récursive pour parcourir les widgets et choisir celui qui correspond au point donné
    return pick_recursive(racine, where);
}
