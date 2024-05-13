//
// Created by ensimag on 5/10/24.
//


#include "ei_event.h"
#include "ei_implementation.h"

event_with_callback* liste_des_events_enregistres = NULL;

void		ei_bind			(ei_eventtype_t		eventtype,
                     ei_widget_t		widget,
                     ei_tag_t		tag,
                     ei_callback_t		callback,
                     void*			user_param) {
    //event_with_callback* evenmt = (event_with_callback*) (&eventtype);

    traitant_t* nouveau_traitant = malloc(sizeof(traitant_t));
    //nouveau_traitant->evenement =  eventtype;
    nouveau_traitant->widget = widget;
    nouveau_traitant->tag = tag;
    nouveau_traitant->callback = callback;
    nouveau_traitant->user_param = user_param;
    nouveau_traitant->next = NULL;

    if (liste_des_events_enregistres == NULL) {
        event_with_callback* new_with_callback = malloc(sizeof(event_with_callback));
        new_with_callback->event = eventtype;
        new_with_callback->liste_des_traitants = nouveau_traitant;
        new_with_callback->next = NULL;
        liste_des_events_enregistres = new_with_callback;
    }
    else {
        event_with_callback* tete = liste_des_events_enregistres;
        while( tete->event != eventtype && tete->next != NULL) {
            tete = tete->next;
        }
        if(tete->next != NULL){
            traitant_t* liste_traitants = tete->liste_des_traitants;
            if(liste_traitants == NULL) {
                tete->liste_des_traitants = nouveau_traitant;
            }
            else {
                while (liste_traitants->next != NULL) {
                    liste_traitants = liste_traitants->next;
                }
                liste_traitants->next = nouveau_traitant;
            }
        }
        else {
            event_with_callback* new_with_callback = malloc(sizeof(event_with_callback));
            new_with_callback->event = eventtype;
            new_with_callback->liste_des_traitants = nouveau_traitant;
            new_with_callback->next = NULL;
            tete->next = new_with_callback;

            }
        }
}


traitant_t* trouve_traitant(ei_eventtype_t eventtype) {
    event_with_callback* tete = liste_des_events_enregistres;
    if (tete == NULL) {
        return NULL;
    }

    while(tete != NULL) {
        if (tete->event == eventtype) {
            return tete->liste_des_traitants;
        }
        tete = tete->next;
    }
    return NULL;
}


// à revoir: il faut supprimer un élément d'une liste chainee
void		ei_unbind		(ei_eventtype_t		eventtype,
                     ei_widget_t		widget,
                     ei_tag_t		tag,
                     ei_callback_t		callback,
                     void*			user_param) {

    /*event_with_callback* event = (event_with_callback*) (&eventtype);
    traitant_t* liste_des_traitants = event->liste_des_traitants;

    while((liste_des_traitants->evenement != eventtype ||
            liste_des_traitants->widget != widget ||
            liste_des_traitants->tag != tag ||
            liste_des_traitants->callback != callback ||
            liste_des_traitants->user_param != user_param) &&
            liste_des_traitants != NULL){

        liste_des_traitants = liste_des_traitants->next;
    }
    if (liste_des_traitants != NULL) {
        traitant_t* a_supprimer = liste_des_traitants;
        liste_des_traitants->next = liste_des_traitants->next != NULL ? liste_des_traitants->next->next: NULL;
    }
*/
}

// il faut faire les fonctions en rouge
//idée pour la fonction get_widget_from_pick_clor:
// creer une liste chainee des widgets avec couleurs-widgets au fur et à mesure de la
// création des widgets et ensuite parcourir cette liste
ei_widget_t get_widget_actuel(ei_event_t* event) {
    ei_point_t pos_souris = event->param.mouse.where;
    //ei_color_t pick_screen_color = get_pick_screen_color(pos_souris);
    //return get_widget_from_pick_color(pick_screen_color);
    return root_widget; // à supprimer après
}

bool execute_traitant(ei_event_t* event,traitant_t traitant) {
    if(traitant.tag == NULL && traitant.widget != NULL) {
        if(get_widget_actuel(event) == traitant.widget) {
            return traitant.callback(traitant.widget, event, traitant.user_param);
        }
        //return true;
    }
    if (traitant.tag != NULL && traitant.widget == NULL){
            if(strcmp(traitant.tag,"all") == 0){
                return traitant.callback(get_widget_actuel(event), event, traitant.user_param);
            }
            if(strcmp(get_widget_actuel(event)->wclass->name,traitant.tag) == 0) {
                    return traitant.callback(get_widget_actuel(event),event, traitant.user_param);

            }
        //return true;

        //return  true;
        }
    return true;
}