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

    uint8_t* pointeur_surface = hw_surface_get_buffer(surface);
    ei_size_t size_surface = hw_surface_get_size(surface);
    int width_surface = size_surface.width;
    int height_surface = size_surface.height;

    ei_surface_t surf = hw_text_create_surface(text, font, color);
    //ei_surface_t surf = hw_image_load("misc/klimt.jpg", surface);
    hw_surface_lock(surf);

    ei_size_t size_surf = hw_surface_get_size(surf);
    int width_surf = size_surf.width;
    int height_surf = size_surf.height;
    uint8_t* pointeur_surf= hw_surface_get_buffer(surf);

    pointeur_surface = pointeur_surface + 4*(where->y * width_surface + where->x);
    //int32_t i = (where->y * width_surface + where->x;
    int i,j;
    int ir, ig, ib, ia;
    int r_s, g_s, b_s, r_d, g_d, b_d, a_s, a_d;

    ei_point_t pos_deb = clipper->top_left;
    ei_size_t dim_clip = clipper->size;

    hw_surface_get_channel_indices(surf, &ir, &ig, &ib, &ia);
    int x_i = where->x;
    int y_i = where->y;
    for(j=0; j < size_surf.height; j++){

        for(i=0;i < size_surf.width;i++) {
            //ei_color_t*  couleur =  map_pick_id_to_color(*pointeur_surf);
            //int pt_y = ((int) pointeur_surface - (int) val_origin) /(size_surface.width *4 );
            //int pt_x = ((int) pointeur_surface- (int) val_origin) % (size_surface.width*4);
            //int valeur_alpha = (*pointeur_surf>>24) & 0xFF;
            //uint8_t tab[4] = {couleur->blue, couleur->green, couleur->red, couleur->alpha};

            if(x_i+i >= pos_deb.x && x_i+i <= pos_deb.x + dim_clip.width && y_i+j >= pos_deb.y && y_i+j <= pos_deb.y + dim_clip.height) {
                r_s = pointeur_surf[ir];
                g_s = pointeur_surf[ig];
                b_s = pointeur_surf[ib];
                a_s = pointeur_surf[ia];

                pointeur_surface[ir] = (pointeur_surface[ir] * (255 - a_s) + r_s * a_s) / 255;
                pointeur_surface[ig] = (pointeur_surface[ig] * (255 - a_s) + g_s * a_s) / 255;
                pointeur_surface[ib] = (pointeur_surface[ib] * (255 - a_s) + b_s * a_s) / 255;
                //pointeur_surface[ia] = (pointeur_surface[ia]*(255 - a_s) + r_s*a_s)/255;
            }
            pointeur_surface += 4;
            pointeur_surf += 4;
            //free(couleur);
        }
        //pointeur_surf++;
        //pointeur_surface++;

        pointeur_surface = pointeur_surface + 4*(width_surface - width_surf);
    }
    hw_surface_unlock(surf);
    hw_surface_free(surf);
    hw_surface_unlock(surface);

}