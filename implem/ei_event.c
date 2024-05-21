//
// Created by ah aharbilh on 07 May 2024.
//
#include "ei_application.h"
#include "ei_event.h"
#include "ei_types.h"
#include "ei_implementation.h"
#include "string.h"

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
    return get_widget_from_pick_color(pick_screen_color);
    //return root_widget; // à supprimer après
}

ei_widget_t get_widget_from_pick_color(ei_color_t* pick_color) {
    /*
    link_widget* debut = liste_des_widgets;
    while (debut!= NULL && (debut->widget->pick_color->red != pick_color.red ||
        debut->widget->pick_color->green != pick_color.green ||
        debut->widget->pick_color->blue != pick_color.blue ||
        debut->widget->pick_color->alpha != pick_color.alpha) ) {
        debut = debut->next;
    }
    return (debut==NULL ? NULL: debut->widget);
    */
    //ei_color_t* color = malloc(sizeof(ei_color_t));
    //*color = pick_color;
    ei_widget_t retour =  pick_recursive(root_widget, pick_color);
    free(pick_color);
    return retour;
}
ei_color_t* get_pick_screen_color(ei_point_t pos_souris) {
    hw_surface_lock(offscreen);
    uint32_t* pointeur_surface = (uint32_t*)hw_surface_get_buffer(offscreen);
    ei_size_t size = hw_surface_get_size(offscreen);
    pointeur_surface = pointeur_surface + (size.width*pos_souris.y + pos_souris.x);
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(offscreen, &ir, &ig, &ib, &ia);
    uint8_t* tab = (uint8_t*)pointeur_surface;
    ei_color_t* couleur = malloc(sizeof(ei_color_t));
    couleur->red = tab[ir];
    couleur->green = tab[ig];
    couleur->blue = tab[ib];
    couleur->alpha = 255;
    //*couleur = (ei_color_t){ pointeur_surface[ir],pointeur_surface[ig],pointeur_surface[ib],0};
    hw_surface_unlock(offscreen);
    return couleur;
}
bool execute_traitant(ei_event_t* event,traitant_t traitant) {

    ei_widget_t le_widget = get_widget_actuel(event);
    if(traitant.tag == NULL && traitant.widget != NULL) {
        if(le_widget == traitant.widget) {
            return traitant.callback(traitant.widget, event, traitant.user_param);
        }
    }
    else if (traitant.tag != NULL && traitant.widget == NULL){
        if(strcmp(traitant.tag,"all") == 0){
            return traitant.callback(le_widget, event, traitant.user_param);
        }
        if( le_widget != NULL && strcmp(le_widget->wclass->name,traitant.tag) == 0) {
            return traitant.callback(le_widget,event, traitant.user_param);
        }
    }
    return false;
}


void remove_letter(char* str, int index) {
    int len = strlen(str);

    if (index >= 0 && index < len) {
        for (int i = index; i < len; i++) {
            str[i] = str[i + 1];
        }
    }
}

void insert_char(char* str, char char_to_insert, int index, int max_len) {
    int len = strlen(str);

    // Ensure index is within bounds and there's space for the new character
    if (index >= 0 && index <= len && len < max_len - 1) {
        // Shift characters to the right
        for (int i = len; i >= index; i--) {
            str[i + 1] = str[i];
        }
        // Insert the new character
        str[index] = char_to_insert;
    }
}

bool entry_handler_1(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    //ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
    ei_widget_t widget_1 = (ei_impl_widget_t*) user_param;
    ei_impl_entry_t* entry = (ei_impl_entry_t*) user_param;
    size_t len = strlen(entry->text);
    int max_cursor_x = widget_1->content_rect->top_left.x + widget_1->content_rect->size.width-2;
    int min_cursor_x = widget_1->content_rect->top_left.x +2;
    if(event->type == ei_ev_text_input) {
        printf("\n\n\nil ya eu keydown\n\n\n");


        if (len <= entry->requested_char_size - 1)
        {
            char lettre = event->param.text;
            if (isprint(lettre)) {

                char lettre_str[1] = { lettre};
                //const char l[1] = "";
                //strncat(entry->text, lettre_str, 1);
                insert_char(entry->text, lettre, entry->ind_cur+1, entry->requested_char_size+1);
                //strncat(l,lettre_str,1);
                //strcat(entry->text, lettre_str);
                int w,h;
                // const ei_const_string_t l = (ei_const_string_t) &lettre;
                hw_text_compute_size(lettre_str,entry->text_font, &w,&h);




                curseur[0].x = min(curseur[0].x + w, max_cursor_x);
                curseur[1].x = min(curseur[1].x + w, max_cursor_x);
                entry->ind_cur += 1;
                if (curseur[0].x >= max_cursor_x) {
                    entry->pos += 1;
                    // mise à jour pos debut
                }


                printf("le text dans entry vaut: %s", entry->text);
            }

            ei_app_invalidate_rect(&widget_1->screen_location);

        }
    }

    else if (event->type == ei_ev_keydown) {
        if (event->param.key_code == SDLK_RIGHT) {
            if(entry->text[entry->ind_cur+1]) {
                int w1, h1;
                char lettre[1] = {entry->text[entry->ind_cur+1]};

                hw_text_compute_size(lettre,entry->text_font, &w1,&h1);
                curseur[0].x =  min(curseur[0].x+w1, max_cursor_x); ;
                curseur[1].x = min(curseur[1].x+w1, max_cursor_x);

                entry->ind_cur = min( entry->ind_cur +1, len-1);
            }

        }

        else if (event->param.key_code == SDLK_LEFT) {
            if(entry->text[entry->ind_cur] && entry->ind_cur >= 0) {
                int w1, h1;
                char lettre[1] = {entry->text[entry->ind_cur]};

                hw_text_compute_size(lettre,entry->text_font, &w1,&h1);
                curseur[0].x =  max(curseur[0].x-w1, min_cursor_x);
                curseur[1].x = max(curseur[1].x-w1, min_cursor_x);

                //entry->ind_cur = max( entry->ind_cur -1, 0);
            }
            entry->ind_cur = max( entry->ind_cur -1, -1);

        }
            /*
            else if(event->param.key_code == SDLK_DELETE) {
                if(entry->text[entry->ind_cur+1]) {
                    int w1, h1;
                    char* text = entry->text + entry->ind_cur+1;
                    remove_letter(text, entry->text[entry->ind_cur+1]);
                    strncpy(entry->text, entry->text, entry->ind_cur);
                    strcat(entry->text, text);
                }

            }*/
        else if (event->param.key_code == SDLK_DELETE) {
            if (entry->text[entry->ind_cur+1] != '\0') {
                int len = strlen(entry->text);

                // Ensure we do not go out of bounds
                if (entry->ind_cur+1 < len) {
                    // Remove the character at the current cursor position
                    remove_letter(entry->text, entry->ind_cur+1);
                }
            }
        }
        else if (event->param.key_code == SDLK_BACKSPACE) {
            if (entry->text[entry->ind_cur] != '\0') {
                int len = strlen(entry->text);

                // Ensure we do not go out of bounds
                if (entry->ind_cur < len) {
                    // Remove the character at the current cursor position

                    int w1, h1;
                    char lettre[1] = {entry->text[entry->ind_cur]};

                    hw_text_compute_size(lettre,entry->text_font, &w1,&h1);
                    curseur[0].x =  max(curseur[0].x-w1, min_cursor_x);
                    curseur[1].x = max(curseur[1].x-w1, min_cursor_x);
                    remove_letter(entry->text, entry->ind_cur);
                    entry->ind_cur = max( entry->ind_cur -1, -1);

                }
            }
        }
        ei_app_invalidate_rect(&widget_1->screen_location);
    }
        /*
        else if (event->type == ei_ev_mouse_buttondown && (void*)widget == (void*)entry) {
            curseur[0].x = event->param.mouse.where.x;
            curseur[1].x = event->param.mouse.where.x;
            ei_app_invalidate_rect(&widget->screen_location);
        }*/
    else if (event->type == ei_ev_mouse_buttondown && (void*)widget != (void*)entry) {

        entry->focus = false;
        ei_unbind(ei_ev_mouse_buttondown, NULL, "all", entry_handler_1, user_param);
        ei_unbind(ei_ev_text_input, NULL, "all", entry_handler_1, user_param);
        ei_unbind(ei_ev_keydown, NULL, "all", entry_handler_1, user_param);
        ei_app_invalidate_rect(&widget->screen_location);
    }

    return true;
}

bool entry_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    event_with_callback* tete = liste_des_events_enregistres;
    if (event->type == ei_ev_mouse_buttondown && strcmp(widget->wclass->name, "entry") == 0) {
        ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
        entry->focus = true;
        ei_bind (ei_ev_text_input, NULL,"all", entry_handler_1, widget);
        ei_bind(ei_ev_mouse_buttondown, NULL, "all", entry_handler_1, widget);
        ei_bind(ei_ev_keydown, NULL, "all", entry_handler_1, widget);

        ei_point_t where;
        //where = place_text(widget, entry->text, entry->text_font, ei_anc_northeast);
        int w, h;
        hw_text_compute_size(entry->text, entry->text_font, &w, &h);
        where.x = widget->content_rect->top_left.x+2+w;
        where.y = widget->content_rect->top_left.y;
        if (entry->text != '\0') {
            int l = strlen((entry->text)) - 1;
            entry->ind_cur = l;
        }


            curseur[0]= where;
        curseur[1]= (ei_point_t){where.x, where.y + widget->requested_size.height};
        ei_app_invalidate_rect(&widget->screen_location);
        return false;
    }
    printf("évènement entry gerer");
    return false;

}