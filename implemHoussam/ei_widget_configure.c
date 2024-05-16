//
// Created by traorefv on 30/04/24.
//

#include "ei_widget_configure.h"
#include "ei_implementation.h"
#include "ei_event.h"
#include "ei_application.h"
#include <stdbool.h>
#include "stdio.h"
#include "ei_placer.h"


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
    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
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
        frame->text = *text;
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
        frame->img_rect = *img_rect;
    }
    if ( !verifie_si_null(img_anchor) ) {
        frame->img_anchor = *img_anchor;
    }
}

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
    } else{
        if (*text) {
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
        button->text = *text;
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
    if ( !verifie_si_null(img) ) {
        button->img = *img;
    }
    if ( !verifie_si_null(img_rect) ) {
        button->img_rect = *img_rect;
    }
    if ( !verifie_si_null(img_anchor) ) {
        button->img_anchor = *img_anchor;
    }
    if ( !verifie_si_null(callback) ) {
        button->callback = *callback;
    }

    if ( !verifie_si_null(user_param) ) {
        button->user_param = *user_param;
    }
    //un problème user_param
    ei_bind(ei_ev_mouse_buttondown, widget, NULL, events_button,(user_param != NULL)?*user_param: NULL);
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, events_button,(user_param != NULL)?*user_param: NULL);
}

bool events_toplevel_down(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    //if (toplevel->resizable != ei_axis_none) {
        if (event->type == ei_ev_mouse_buttondown) {
            printf("\n mouse down on toplevel\n ");
            down_toplevel = true;
            pt_init_toplevel = event->param.mouse.where;
            return true;
        } else if (event->type == ei_ev_mouse_buttonup) {
            down_toplevel = false;
            printf("\n mouse up from toplevel\n ");
            return true;
        }
    //}
    return false;
}

bool events_toplevel_place(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    //ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    if (event->type == ei_ev_mouse_move && down_toplevel == true) {
        printf("\n mouse is moving on toplevel\n");
        ei_point_t pt_last_toplevel = event->param.mouse.where;
        int new_x = widget->screen_location.top_left.x + (pt_last_toplevel.x - pt_init_toplevel.x);
        int new_y = widget->screen_location.top_left.y + (pt_last_toplevel.y - pt_init_toplevel.y);
        new_x = (new_x>0 && new_x<(root_widget->requested_size.width - widget->requested_size.width))?new_x:0;
        new_y = (new_y>0 && new_y< (root_widget->requested_size.height - widget->requested_size.height))?new_y:0;

        ei_place_xy(widget, new_x, new_y>0?new_y:0);
        pt_init_toplevel = event->param.mouse.where;
    }
    return true;
}


/*
           switch (toplevel->resizable) {
               case ei_axis_both:
                   toplevel->widget.screen_location.top_left.x += new_x;
                   toplevel->widget.screen_location.top_left.y += new_y;
                   break;
               case ei_axis_x:
                   toplevel->widget.screen_location.top_left.x += new_x;
                   break;
               case ei_axis_y:
                   toplevel->widget.screen_location.top_left.y += new_y;
                   break;
               case ei_axis_none:
                   break;
           }*/
//ei_app_invalidate_rect(&widget->screen_location);
void			ei_toplevel_configure		(ei_widget_t		widget,
                                              ei_size_t*		requested_size,
                                              const ei_color_t*	color,
                                              int*			border_width,
                                              ei_string_t*		title,
                                              bool*			closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_ptr_t*		min_size) {

    if (requested_size != NULL) {
        widget->requested_size = *requested_size;
        widget->content_rect->size = *requested_size;
    }

    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;

    if (color != NULL) {
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
}
