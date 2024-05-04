//
// Created by traorefv on 30/04/24.
//

#include "ei_widget.h"
#include "ei_implementation.h"
#include "ei_widgetclass.h"

ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                            ei_widget_t		parent,
                                            ei_user_param_t	user_data,
                                            ei_widget_destructor_t destructor){

    if (parent == NULL) {
        return NULL;
    }
    // la classe du widget à creer; on suppose ici qu'elle existe
    ei_widgetclass_t* classe_du_widget = ei_widgetclass_from_name(class_name);
    // creation d'une instance de widget:

    ei_widget_t new_widget = classe_du_widget->allocfunc();
    classe_du_widget->setdefaultsfunc(new_widget);

    // pour initialiser les parametres qui sont communs à tous,
    // on fait a pas besoin de faire de transcriptage car ils est considerer d'abord commme du type
    // ei_impl_widget_t

    new_widget->parent = parent;
    parent->children_head = new_widget;
    // il faudra ici bien faire les liaisons entres les fils et fréres de widget
    // j'y reviendrai
    //new_widget->next_sibling =
    new_widget->destructor = destructor;
    new_widget->user_data = user_data;

    return new_widget;

}

