//
// Created by traorefv on 30/04/24.
//

#include "ei_widget_configure.h"

#include <ei_application.h>

#include "ei_implementation.h"
#include <stdbool.h>


bool verifie_si_null(void* chose_a_tester){
    return (chose_a_tester == NULL);
}
void			ei_frame_configure		(ei_widget_t		widget,
                                           ei_size_t*		requested_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_relief_t*		relief,
                                           ei_string_t*		text,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color,
                                           ei_anchor_t*		text_anchor,
                                           ei_surface_t*		img,
                                           ei_rect_ptr_t*		img_rect,
                                           ei_anchor_t*		img_anchor){



    widget = (ei_widget_t) widget;
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    if ( !verifie_si_null(text_font) ) {
        frame->text_font = *text_font;
    }

    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
        if(widget->geom_params != NULL) {
            ei_place_wh(widget,requested_size->width, requested_size->height);
        }
        //widget->screen_location.size = *requested_size;
        if(widget == root_widget) {
            widget->screen_location.size = *requested_size;
        }
    }
    else{
        if (text != NULL) {

            int width_text;
            int height_text;
            hw_text_compute_size( *text,  (((ei_impl_frame_t*)widget)->text_font), &width_text, &height_text);
            widget->requested_size = (ei_size_t){width_text,height_text};
        }
    }

    // on fait un transcriptage pour avoir accés aux autres champs


    if ( color != NULL){
        frame->color = *color;
    }

    if ( !verifie_si_null(border_width) ) {
        frame->border_width = *border_width;
    }
    if ( !verifie_si_null(relief) ) {
        frame->relief = *relief;
    }
    if ( !verifie_si_null(text) ) {

        /*
        if(frame->text != NULL) {
            free(frame->text);
        }
        frame->text = (char*)malloc(strlen(*text) + 1);
        if (frame->text == NULL) {
            printf("Erreur d'allocation de mémoire pour le texte.\n");
            return;
        }

        // Copier le texte dans le widget
        strcpy(frame->text, *text);
        */
        frame->text = strdup(*text);
    }

    if ( !verifie_si_null(text_color) ) {
        frame->text_color = *text_color;
    }
    if ( !verifie_si_null(text_anchor) ) {
        frame->text_anchor = *text_anchor;
    }
    if ( !verifie_si_null(img) ) {
        frame->img = *img;
    }
    if ( !verifie_si_null(img_rect) ) {
        if (frame->img_rect){
            free(frame->img_rect);
        }
        ei_rect_t *new_img_rect = malloc(sizeof(ei_rect_t));
        *new_img_rect = *(*img_rect);
        frame->img_rect = new_img_rect;
    }
    if ( !verifie_si_null(img_anchor) ) {
        frame->img_anchor = *img_anchor;
    }

    ei_app_invalidate_rect(&widget->screen_location);
}

// bouton configure

void			ei_button_configure		(ei_widget_t		widget,
                                            ei_size_t*		requested_size,
                                            const ei_color_t*	color,
                                            int*			border_width,
                                            int*			corner_radius,
                                            ei_relief_t*		relief,
                                            ei_string_t*		text,
                                            ei_font_t*		text_font,
                                            ei_color_t*		text_color,
                                            ei_anchor_t*		text_anchor,
                                            ei_surface_t*		img,
                                            ei_rect_ptr_t*		img_rect,
                                            ei_anchor_t*		img_anchor,
                                            ei_callback_t*		callback,
                                            ei_user_param_t*	user_param){



    ei_impl_button_t* button = (ei_impl_button_t*) widget;

    if ( !verifie_si_null(text_font) ) {
        button->text_font = *text_font;
    }
    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
    }

    else{
        if (text != NULL) {

        int width_text;
        int height_text;
        hw_text_compute_size( *text,  (((ei_impl_button_t*)widget)->text_font), &width_text, &height_text);
        widget->requested_size = (ei_size_t){width_text,height_text};


    }
    }

    // on fait un transcriptage pour avoir accés aux autres champs


    if ( color != NULL){
        button->color = *color;
    }

    if ( !verifie_si_null(border_width) ) {
        button->border_width = *border_width;
    }
    if ( !verifie_si_null(corner_radius) ) {
        button->corner_radius = *corner_radius;
    }
    if ( !verifie_si_null(relief) ) {
        button->relief = *relief;
    }
    if ( !verifie_si_null(text) ) {
        button->text = strdup(*text);
    }

    if ( !verifie_si_null(text_color) ) {
        button->text_color = *text_color;
    }
    if ( !verifie_si_null(text_anchor) ) {
        button->text_anchor = *text_anchor;
    }
    /*
    if ( !verifie_si_null(img_rect) ) {
        if (button->img_rect){
            free(button->img_rect);
        }
        ei_rect_t* new_img_rect = malloc(sizeof(ei_rect_t));
        if(new_img_rect == NULL) {
            free(new_img_rect);
            exit(EXIT_FAILURE);
        }
        *new_img_rect = *(*img_rect);
        button->img_rect = new_img_rect;
    }
    if ( !verifie_si_null(img) ) {
        if(button->img) {
            hw_surface_free(button->img);
        }

        ei_size_t taille = button->img_rect->size;
        ei_surface_t copie = hw_surface_create(root_window,taille,false);
        ei_size_t taille_1 = hw_surface_get_size(copie);
        int retour = ei_copy_surface(copie,NULL,*img,*img_rect, false);
        if(retour == 1) {
            printf("\n\n\n ECHEC COPIE SURFACES\n\n\n");
            exit(EXIT_FAILURE);
        }
        ei_size_t taille_2 = hw_surface_get_size(copie);
        button->img =copie;

    }
    */

    if (!verifie_si_null(img_rect)) {
        if (button->img_rect) {
            free(button->img_rect);
        }
        ei_rect_t *new_img_rect = malloc(sizeof(ei_rect_t));
        if (new_img_rect == NULL) {
            fprintf(stderr, "Memory allocation failed for new_img_rect\n");
            exit(EXIT_FAILURE);
        }
        *new_img_rect = *(*img_rect);
        button->img_rect = new_img_rect;
    }

    if (!verifie_si_null(img)) {
        if (button->img) {
            hw_surface_free(button->img);
        }

        if (button->img_rect) {
            ei_size_t taille = button->img_rect->size;
            ei_surface_t copie = hw_surface_create(root_window, taille, false);
            if (copie == NULL) {
                fprintf(stderr, "Failed to create surface\n");
                exit(EXIT_FAILURE);
            }

            int retour = ei_copy_surface(copie, NULL, *img, button->img_rect, false);
            if (retour != 0) {
                printf("\n\n\n ECHEC COPIE SURFACES\n\n\n");
                hw_surface_free(copie);
                exit(EXIT_FAILURE);
            }
            button->img_rect->top_left = (ei_point_t){0,0};
            button->img = copie;
        } else {
            fprintf(stderr, "img_rect is NULL while img is not\n");
            exit(EXIT_FAILURE);
        }
    }

    if ( !verifie_si_null(img_anchor) ) {
        button->img_anchor = *img_anchor;
    }
    if ( !verifie_si_null(callback) ) {
        button->callback = *callback;
        ei_bind(ei_ev_mouse_buttondown,widget,NULL,*callback,(user_param != NULL)?*user_param: NULL);
    }

    if ( !verifie_si_null(user_param) ) {
        button->user_param = *user_param;
    }

    ei_app_invalidate_rect(&widget->screen_location);

}

// configuratin d'un widget
void			ei_toplevel_configure		(ei_widget_t		widget,
                                              ei_size_t*		requested_size,
                                              const ei_color_t*	color,
                                              int*			border_width,
                                              ei_string_t*		title,
                                              bool*			closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_ptr_t*		min_size) {



    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;

    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
        widget->content_rect->size = *requested_size;
    }

    if(color != NULL) {
        toplevel->color = *color;
    }
    if (!verifie_si_null(border_width)) {
        toplevel->border_width = *border_width;
    }
    if (!verifie_si_null(title)) {
        toplevel->title = *title;
    }
    if (!verifie_si_null(closable)) {
        toplevel->closable = *closable;
    }
    if (!verifie_si_null(min_size)) {
        toplevel->min_size = *min_size;
    }

    if (!verifie_si_null(resizable)) {
        toplevel->resizable = *resizable;
    }

    ei_app_invalidate_rect(&widget->screen_location);
/*
        if(*resizable != ei_axis_none) {
            ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel",toplevel_redimension, NULL);
        }
    }
    else {
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel",toplevel_redimension, NULL);
    }*/

}


