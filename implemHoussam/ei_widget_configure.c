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

bool verifie_si_null_text(const char* text) {
    return (text == NULL || text[0] == '\0');
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

    // on fait un transcriptage pour avoir accés aux autres champs
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;

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
/*    if (text != NULL && !verifie_si_null_text(*text)) {
        frame->text = strdup(*text);
        if (frame->text == NULL) {
            printf("Erreur Impossible d'allouer \n");
            exit(EXIT_FAILURE);
        }
    } else {
        frame->text = NULL;
    }*/
        frame->text = strdup(*text);
    }
    if ( !verifie_si_null(text_font) ) {
        frame->text_font = *text_font;
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

// bouton configure
bool events_button(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    /*ei_impl_button_t * button = (ei_impl_button_t *) widget;
    if (event != NULL) {
        if (event->type == ei_ev_mouse_buttondown && ei_widget_pick(&(event->param.mouse.where)) == button) {
            button->relief = ei_relief_sunken;
            //ei_app_invalidate_rect(widget->content_rect);
            (*(root_widget->wclass->drawfunc))(root_widget, root_window, offscreen, root_widget->content_rect);
            hw_surface_update_rects(root_window, NULL);
            printf("appui");
            return true;
        } else if (event->type == ei_ev_mouse_buttonup) {
            button->relief = ei_relief_raised;
            //ei_app_invalidate_rect(widget->content_rect);
            return true;
        }
    } else {
        button->relief = ei_relief_none;
        return false;
    }*/
    ei_impl_button_t* bouton = (ei_impl_button_t*) widget;
    if (event->type == ei_ev_mouse_buttondown) {
        bouton->relief = ei_relief_sunken;
        printf("\n Button appuyé \n");
        ei_app_invalidate_rect(&widget->screen_location);
        return false;
    }
    else if (event->type == ei_ev_mouse_buttonup) {
        bouton->relief = ei_relief_raised;
        printf("\n Button relaché \n");
        ei_app_invalidate_rect(&widget->screen_location);
        return false;
    }
    return true;
}


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



    widget = (ei_widget_t) widget;
    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
    }

    else{
        if (  *text) {

            int width_text;
            int height_text;
            hw_text_compute_size( *text,  (((ei_impl_button_t*)widget)->text_font), &width_text, &height_text);
            widget->requested_size = (ei_size_t){width_text,height_text};


        }
    }

    // on fait un transcriptage pour avoir accés aux autres champs
    ei_impl_button_t* button = (ei_impl_button_t*) widget;

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
    if ( !verifie_si_null(text_font) ) {
        button->text_font = *text_font;
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
        //ei_bind(ei_ev_mouse_buttondown, widget, NULL, events_button,(user_param != NULL)?*user_param: NULL);
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, button->callback,(user_param != NULL)?*user_param: NULL);

    } else {
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, events_button,(user_param != NULL)?*user_param: NULL);
        ei_bind(ei_ev_mouse_buttonup, widget, NULL, events_button,(user_param != NULL)?*user_param: NULL);
    }

    if ( !verifie_si_null(user_param) ) {
        button->user_param = *user_param;
    }

    ei_app_invalidate_rect(&widget->screen_location);

}

// configuratin d'un toplevel

bool events_toplevel_down(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    ei_point_t pos_souri = event->param.mouse.where;
    ei_point_t coin_gauche = widget->screen_location.top_left;
    ei_size_t taille = widget->screen_location.size;
    ei_point_t pos_carre;
    pos_carre.x = widget->screen_location.top_left.x + widget->screen_location.size.width+ 2*toplevel->border_width -10;
    pos_carre.y = widget->screen_location.top_left.y + widget->screen_location.size.height + 10 + toplevel->border_width;

    //if (toplevel->resizable != ei_axis_none) {
    if (event->type == ei_ev_mouse_buttondown &&
        (pos_souri.x >= coin_gauche.x) &&
        (pos_souri.x <= coin_gauche.x+ taille.width) && (pos_souri.y <= coin_gauche.y+20)  &&
        (pos_souri.y >= coin_gauche.y)) {
        printf("\n mouse down on top of toplevel\n ");
        top_toplevel = true;
        pt_init_toplevel = event->param.mouse.where;
        return false;
    } else if (event->type == ei_ev_mouse_buttondown && (pos_souri.x >= pos_carre.x) &&
               (pos_souri.x <= pos_carre.x+ 10) && (pos_souri.y <= pos_carre.y+10)  &&
               (pos_souri.y >= pos_carre.y)){
        printf("\n mouse down on bottom of toplevel\n ");
        btm_toplevel = true;
        pt_init_toplevel = event->param.mouse.where;
        return false;
    } else if (event->type == ei_ev_mouse_buttonup) {
        top_toplevel = false;
        btm_toplevel = false;
        printf("\n mouse up from toplevel\n ");
        return true;
    }
    //}
    return false;
}

bool events_toplevel_place(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    int new_x, new_y;
    int new_width, new_height;
    ei_point_t pt_last_toplevel = event->param.mouse.where;

    if (event->type == ei_ev_mouse_move && top_toplevel == true) {
        printf("\n mouse is moving on toplevel\n");
        new_x = widget->screen_location.top_left.x + (pt_last_toplevel.x - pt_init_toplevel.x);
        new_y = widget->screen_location.top_left.y + (pt_last_toplevel.y - pt_init_toplevel.y);
        new_x = (new_x>0 && new_x<(root_widget->requested_size.width - widget->requested_size.width))?new_x:0;
        new_y = (new_y>0 && new_y<(root_widget->requested_size.height - widget->requested_size.height))?new_y:0;
        ei_place_xy(widget, new_x, new_y>0?new_y:0);
        pt_init_toplevel = event->param.mouse.where;
    } else if (event->type == ei_ev_mouse_move && btm_toplevel == true){
        new_x =  pt_last_toplevel.x - pt_init_toplevel.x;
        new_y =  pt_last_toplevel.y - pt_init_toplevel.y;
        new_width = widget->screen_location.size.width + new_x;
        new_height = widget->screen_location.size.height + new_y;
        switch (toplevel->resizable) {
            case ei_axis_both:
                ei_place_wh(widget, new_width, new_height);
                break;
            case ei_axis_x:
                ei_place(widget,NULL,NULL, NULL, &new_width, NULL,NULL,NULL,NULL,NULL);
                break;
            case ei_axis_y:
                ei_place(widget,NULL,NULL, NULL, NULL, &new_height,NULL,NULL,NULL,NULL);
                break;
            case ei_axis_none:
                break;
        }
        surfaces_mise_a_jour = NULL;

    }
    pt_init_toplevel = event->param.mouse.where;
    return false;
}

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
    ei_bind(ei_ev_mouse_buttondown, widget, NULL, events_toplevel_down, NULL);
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, events_toplevel_down, NULL);
    ei_bind(ei_ev_mouse_move, widget, NULL, events_toplevel_place, NULL);

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
