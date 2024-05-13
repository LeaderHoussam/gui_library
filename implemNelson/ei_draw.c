//
// Created by noslen on 5/9/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "ei_widgetclass.h"

#include <ei_widget_attributes.h>
#include <locale.h>

#include "ei_implementation.h"
#include "ei_draw.h"

#include "hw_interface.h"

void	ei_draw_text	(ei_surface_t		surface,
                         const ei_point_t*	where,
                         ei_const_string_t	text,
                         ei_font_t		font,
                         ei_color_t		color,
                         const ei_rect_t*	clipper){

    hw_surface_lock(surface);

    uint32_t* pointeur_surface = (uint32_t*) hw_surface_get_buffer(surface);
    ei_size_t size_surface = hw_surface_get_size(surface);
    int width_surface = size_surface.width;
    int height_surface = size_surface.height;

    ei_surface_t surf = hw_text_create_surface(text, font, color);
    hw_surface_lock(surf);

    ei_size_t size_surf = hw_surface_get_size(surf);
    int width_surf = size_surf.width;
    int height_surf = size_surf.height;
    uint32_t* pointeur_surf= (uint32_t*) hw_surface_get_buffer(surf);

    pointeur_surface = pointeur_surface + (where->y * width_surface + where->x);
    int i;
    int j;

    for(j=0; j < size_surf.height; j++){

        for(i=0;i < size_surf.width;i++) {

            int valeur_alpha = (*pointeur_surf>>24) & 0xFF;

            if(valeur_alpha != 0 ) {
                *pointeur_surface = *(pointeur_surf);
            }

            pointeur_surface++;
            pointeur_surf++;

        }

        pointeur_surface = pointeur_surface + width_surface - width_surf ;
    }
    hw_surface_unlock(surf);
    hw_surface_free(surf);
    hw_surface_unlock(surface);

}