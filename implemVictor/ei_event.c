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
void ei_unbind(ei_eventtype_t eventtype,
               ei_widget_t widget,
               ei_tag_t tag,
               ei_callback_t callback,
               void* user_param) {

    // Parcours de la liste des événements enregistrés
    event_with_callback* event_ptr = liste_des_events_enregistres;
    while (event_ptr != NULL && event_ptr->event != eventtype) {
        event_ptr = event_ptr->next;
    }

    if (event_ptr != NULL) {
        // Parcours de la liste des traitants de l'événement trouvé
        traitant_t* traitant_ptr = event_ptr->liste_des_traitants;
        traitant_t* previous = NULL;
        while (traitant_ptr != NULL) {
            // Vérification si le traitant correspond aux paramètres donnés
            if (traitant_ptr->widget == widget &&
                traitant_ptr->tag == tag &&
                traitant_ptr->callback == callback &&
                traitant_ptr->user_param == user_param) {
                // Suppression du traitant de la liste
                if (previous != NULL) {
                    previous->next = traitant_ptr->next;
                } else {
                    event_ptr->liste_des_traitants = traitant_ptr->next;
                }
                free(traitant_ptr);
                return; // Sortie de la fonction une fois que le traitant est supprimé
                }
            previous = traitant_ptr;
            traitant_ptr = traitant_ptr->next;
        }
    }
}


// il faut faire les fonctions en rouge
//idée pour la fonction get_widget_from_pick_clor:
// creer une liste chainee des widgets avec couleurs-widgets au fur et à mesure de la
// création des widgets et ensuite parcourir cette liste
ei_widget_t get_widget_actuel(ei_event_t* event) {
    ei_point_t pos_souris = event->param.mouse.where;
    ei_size_t taille_max = root_widget->screen_location.size;
    if(pos_souris.x > taille_max.width || pos_souris.y > taille_max.height) {
        return NULL;
    }
    ei_color_t* pick_screen_color = get_pick_screen_color(pos_souris);
    return get_widget_from_pick_color(*pick_screen_color);
    //return root_widget; // à supprimer après
}
ei_widget_t get_widget_from_pick_color(ei_color_t pick_color) {
    link_widget* debut = liste_des_widgets;
    while (debut!= NULL && (debut->widget->pick_color->red != pick_color.red ||
        debut->widget->pick_color->green != pick_color.green ||
        debut->widget->pick_color->blue != pick_color.blue ||
        debut->widget->pick_color->alpha != pick_color.alpha) ) {
        debut = debut->next;
    }
    return (debut==NULL ? NULL: debut->widget);
}
ei_color_t* get_pick_screen_color(ei_point_t pos_souris) {
    hw_surface_lock(surface_arriere);
    uint32_t* pointeur_surface = (uint32_t*)hw_surface_get_buffer(surface_arriere);
    ei_size_t size = hw_surface_get_size(surface_arriere);
    pointeur_surface = pointeur_surface + (size.width*pos_souris.y + pos_souris.x);
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface_arriere, &ir, &ig, &ib, &ia);
    uint8_t* tab = (uint8_t*)pointeur_surface;
    ei_color_t* couleur = malloc(sizeof(ei_color_t));
    couleur->red = tab[ir];
    couleur->green = tab[ig];
    couleur->blue = tab[ib];
    couleur->alpha = 255;
    //*couleur = (ei_color_t){ pointeur_surface[ir],pointeur_surface[ig],pointeur_surface[ib],0};
    hw_surface_unlock(surface_arriere);
    return couleur;
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
            if( get_widget_actuel(event) != NULL && strcmp(get_widget_actuel(event)->wclass->name,traitant.tag) == 0) {
                    return traitant.callback(get_widget_actuel(event),event, traitant.user_param);

            }
        //return true;

        //return  true;
        }
    return false;
    //return true;

    //return traitant.callback(root_widget->children_head, event, traitant.user_param);
}