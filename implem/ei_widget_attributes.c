//
// Created by ensimag on 5/14/24.
//

#include "ei_widget_attributes.h"
#include "ei_implementation.h"

void* ei_widget_get_user_data(ei_widget_t widget) {
    return widget->user_data;
}

ei_widget_t 		ei_widget_get_last_child	(ei_widget_t		widget) {
    return widget->children_tail;
}

ei_widget_t 		ei_widget_get_first_child	(ei_widget_t		widget) {
    return widget->children_head;
}

ei_widget_t 		ei_widget_get_next_sibling	(ei_widget_t		widget) {
    return widget->next_sibling;
}

ei_widgetclass_t*	ei_widget_get_class		(ei_widget_t		widget) {
    return widget->wclass;
}

ei_widget_t 		ei_widget_get_parent		(ei_widget_t		widget) {
    if (widget != root_widget) {
        return widget->parent;
    }
    return NULL;
}

const ei_rect_t*	ei_widget_get_screen_location	(ei_widget_t		widget) {
    return &widget->screen_location;
}

const ei_rect_t*	ei_widget_get_content_rect	(ei_widget_t		widget) {
    return widget->content_rect;
}

void	 		ei_widget_set_content_rect	(ei_widget_t		widget,
                                               const ei_rect_t*	content_rect) {

    if(widget->content_rect != NULL){
        widget->content_rect->top_left = content_rect->top_left;
        widget->content_rect->size = content_rect->size;
    }
    else {
        ei_rect_t* new_content_rect = malloc(sizeof(ei_rect_t));
        new_content_rect->top_left =  content_rect->top_left;
        new_content_rect->size = content_rect->size;

        widget->content_rect = new_content_rect;
    }

}

const ei_color_t*	ei_widget_get_pick_color	(ei_widget_t		widget) {
    return widget->pick_color;
}

const ei_size_t*	ei_widget_get_requested_size	(ei_widget_t		widget) {
    return &widget->requested_size;
}

void	 		ei_widget_set_requested_size	(ei_widget_t		widget,
                                                 ei_size_t 		requested_size) {
    widget->requested_size = requested_size;
}
