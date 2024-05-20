//
// Created by ah aharbilh on 07 May 2024.
//
#include "ei_event.h"
#include "ei_types.h"
#include "ei_implementation.h"
#include "string.h"

ei_event_binding* EVENT_BINDINGS = NULL;

void		ei_bind			(ei_eventtype_t		eventtype,
                                ei_widget_t		widget,
                                ei_tag_t		tag,
                                ei_callback_t		callback,
                                void*			user_param){
   /* if (callback == NULL){
        return; //si le callback n'est pas valide.
    }*/
    if (widget != NULL && tag != NULL){
        return;
    }
    ei_event_binding* new_binding = malloc(sizeof(struct ei_event_binding));
    if (new_binding) {
        new_binding->event_type = eventtype;
        new_binding->widget = widget;
        new_binding->tag = tag;
        new_binding->callback = callback;
        new_binding->user_param = user_param;
        new_binding->next = NULL;
        /*if (EVENT_BINDINGS == NULL){
            EVENT_BINDINGS = new_binding;
        } else {
            ei_event_binding* liste_bind = EVENT_BINDINGS;
            while (liste_bind->next != NULL){
                liste_bind =liste_bind->next;
            }
            liste_bind->next = new_binding;
        }*/
        // au lieu de parcourir toute la liste des events, on ajoute le nouveau à la tête de la liste
        if (EVENT_BINDINGS != NULL){
            ei_event_binding* events_copy = EVENT_BINDINGS;
            new_binding->next = events_copy;
        }
        EVENT_BINDINGS = new_binding;
    }
    }
//free(widget)
void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param){
    struct ei_event_binding *prev_binding = NULL, *current_binding = EVENT_BINDINGS;

    while (current_binding) {
        if (current_binding->event_type == eventtype &&
            current_binding->callback == callback &&
            (current_binding->widget == widget || (current_binding->tag && strcmp(current_binding->tag, tag) == 0))) {
            // Supprimer la liaison courante de la liste
            if (prev_binding)
                prev_binding->next = current_binding->next;
            else
                EVENT_BINDINGS = current_binding->next;

            // Libérer la mémoire allouée pour la liaison supprimée
            free(current_binding);
            return;
        }

        // Passer à la liaison suivante
        prev_binding = current_binding;
        current_binding = current_binding->next;
    }
}

bool execute_traitant(ei_event_t* event, ei_event_binding traitant) {
    if(traitant.tag == NULL && traitant.widget != NULL) {
        if(ei_widget_pick(&(event->param.mouse.where)) == traitant.widget) {
            return traitant.callback(traitant.widget, event, traitant.user_param);
        }
        //return true;
    }
    else if (traitant.tag != NULL && traitant.widget == NULL){
        if(strcmp(traitant.tag,"all") == 0){
            return traitant.callback(ei_widget_pick(&(event->param.mouse.where)), event, traitant.user_param);
        }
        if( ei_widget_pick(&(event->param.mouse.where)) != NULL && strcmp(ei_widget_pick(&(event->param.mouse.where))->wclass->name,traitant.tag) == 0) {
            return traitant.callback(ei_widget_pick(&(event->param.mouse.where)),event, traitant.user_param);

        }
        //return true;
    }
    return false;
    //return true;

    //return traitant.callback(root_widget->children_head, event, traitant.user_param);
}
