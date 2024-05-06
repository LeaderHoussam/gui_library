//
// Created by ensimag on 5/6/24.
//
#include "ei_implementation.h"

#include "ei_application.h"

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                         ei_surface_t		surface,
                         ei_surface_t		pick_surface,
                         ei_rect_t*		clipper) {
    if (widget->children_head != NULL) {
        widget->children_head->wclass->drawfunc(widget->children_head, surface, pick_surface, clipper);
    }
}