//
// Created by ah aharbilh on 07 May 2024.
//
#include "ei_event.h"
#include "ei_types.h"
#include "ei_implementation.h"


void		ei_bind			(ei_eventtype_t		eventtype,
                                ei_widget_t		widget,
                                ei_tag_t		tag,
                                ei_callback_t		callback,
                                void*			user_param){
   /* if (callback == NULL){
        return; //si le callback n'est pas valide.
    }*/
    struct ei_event_binding* new_binding = malloc(sizeof(struct ei_event_binding));
    if (new_binding) {
        new_binding->event_type = eventtype;
        new_binding->widget = &widget;
        new_binding->tag = tag;
        new_binding->callback = callback;
        new_binding->user_param = user_param;
        new_binding->next = EVENT_BINDINGS;
        EVENT_BINDINGS = new_binding;
    }
}

void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param){
    struct ei_event_binding *prev_binding = NULL, *current_binding = EVENT_BINDINGS;

    while (current_binding) {
        if (current_binding->event_type == eventtype &&
            current_binding->callback == callback &&
            (current_binding->widget == &widget || (current_binding->tag && strcmp(current_binding->tag, tag) == 0))) {
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
