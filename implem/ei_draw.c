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

#include <ei_utils.h>


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
    ei_size_t taille_prin = hw_surface_get_size(root_window);
    for(j=0; j < size_surf.height; j++){

        for(i=0;i < size_surf.width;i++) {
            //ei_color_t*  couleur =  map_pick_id_to_color(*pointeur_surf);
            //int pt_y = ((int) pointeur_surface - (int) val_origin) /(size_surface.width *4 );
            //int pt_x = ((int) pointeur_surface- (int) val_origin) % (size_surface.width*4);
            //int valeur_alpha = (*pointeur_surf>>24) & 0xFF;
            //uint8_t tab[4] = {couleur->blue, couleur->green, couleur->red, couleur->alpha};

            //x_i+i >= 0 &&  y_i+j>=0 &&
            if(x_i+i >= 0 &&  y_i+j>=0 && x_i+i < taille_prin.width && y_i+j < taille_prin.height&&
               x_i+i >= pos_deb.x && x_i+i <= pos_deb.x + dim_clip.width && y_i+j >= pos_deb.y && y_i+j <= pos_deb.y + dim_clip.height) {
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

void	ei_fill			(ei_surface_t		surface,
                            const ei_color_t*	color,
                            const ei_rect_t*	clipper) {

   /* int w = clipper->size.width;
    int h = clipper->size.height;
    int x = clipper->top_left.x;
    int y = clipper->top_left.y;*/

    ei_color_t black = {0,0,0,255};
    ei_color_t couleur = (color == NULL)?black: *color;
    /*
    int w = clipper->size.width;
    int h = clipper->size.height;
    int x = clipper->top_left.x;
    int y = clipper->top_left.y;
     */
    hw_surface_lock(surface);
    ei_rect_t surf_rect = hw_surface_get_rect(surface);

    int w = surf_rect.size.width;
    int h = surf_rect.size.height;
    int x = surf_rect.top_left.x;
    int y = surf_rect.top_left.y;

    ei_point_t coins[4] = {surf_rect.top_left,(ei_point_t){x+w,y},(ei_point_t){x+w,y+h} ,(ei_point_t){x,y+h}};
    ei_draw_polygon(surface,coins,4,couleur,clipper);
    hw_surface_unlock(surface);
}



void copy_surface (ei_surface_t source, const ei_point_t origine_src, ei_surface_t destination, const ei_point_t origine_dst, ei_size_t size, bool alpha){
    //on suppose que les deux sources sont de même taille "à partir des origines considérées" et que toute la surface est utilisée
    hw_surface_lock(source);
    hw_surface_lock(destination);


    ei_size_t size_src = hw_surface_get_size( source);
    ei_size_t size_dst = hw_surface_get_size( destination);



    hw_surface_set_origin(destination, origine_dst);
    uint32_t* ptr_dst =(uint32_t*) hw_surface_get_buffer(destination);

    uint32_t* ptr_src = (uint32_t*) hw_surface_get_buffer(source);
    ptr_src = ptr_src + (size_src.width * origine_src.y +origine_src.x);


    int i, cpt;
    int j;
    int ir, ig, ib, ia;
    int r_s, g_s, b_s, r_d, g_d, b_d, a_s, a_d;

    if (alpha == false){a_s = 255;}


    hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);

    for(j=0; j < size.height; j++){

        for(i=0;i < size.width;i++) {


            uint8_t* dst = (uint8_t*)ptr_dst;
            uint8_t* src = (uint8_t*)ptr_src;

            dst[ir] = src[ir];
            dst[ig] = src[ig];
            dst[ib] = src[ib];
            dst[ia] = src[ia];

            ptr_dst++;
            ptr_src++;
        }
        ptr_dst = ptr_dst + (size_dst.width - size.width );
        ptr_src = ptr_src + (size_src.width - size.width );

    }
    ei_point_t origine_zero = {0,0};
    //hw_surface_set_origin(source, origine_zero);
    hw_surface_set_origin(destination, origine_zero);

    hw_surface_unlock(source);
    hw_surface_unlock(destination);

}


int	ei_copy_surface		(ei_surface_t		destination,
                               const ei_rect_t*	dst_rect,
                               ei_surface_t		source,
                               const ei_rect_t*	src_rect,
                               bool			alpha) {

    ei_size_t size_source = hw_surface_get_size(source);
    ei_size_t size_destination = hw_surface_get_size(destination);

    ei_rect_t source_rect = hw_surface_get_rect(source);
    ei_rect_t destination_rect = hw_surface_get_rect(destination);

    if (src_rect == NULL && dst_rect == NULL) {
        if (size_source.width != size_destination.width || size_source.height != size_destination.height) {
            return 1;
        } else if (size_source.width == size_destination.width && size_source.height == size_destination.height) {
            const ei_point_t origine_src = source_rect.top_left;
            const ei_point_t origine_dst = destination_rect.top_left;
            copy_surface(source, origine_src, destination, origine_dst, destination_rect.size, alpha);
            return 0;
        }

    } else if (src_rect == NULL && dst_rect != NULL) {
        if (size_source.width != dst_rect->size.width || size_source.height != dst_rect->size.height) {
            return 1;
        } else if (size_source.width == dst_rect->size.width && size_source.height == dst_rect->size.height) {
            const ei_point_t origine_src = source_rect.top_left;
            const ei_point_t origine_dst = dst_rect->top_left;
            copy_surface(source, origine_src, destination, origine_dst, dst_rect->size, alpha);
            return 0;
        }

    } else if (src_rect != NULL && dst_rect == NULL) {
        if (src_rect->size.width != size_destination.width || src_rect->size.height != size_destination.height) {
            return 1;
        } else if (src_rect->size.width == size_destination.width && src_rect->size.height == size_destination.height) {
            const ei_point_t origine_src = src_rect->top_left;
            const ei_point_t origine_dst = destination_rect.top_left;
            copy_surface(source, origine_src, destination, origine_dst, size_destination, alpha);
            return 0;
        }

    } else if (src_rect != NULL && dst_rect != NULL) {
        if (src_rect->size.width != dst_rect->size.width || src_rect->size.height != dst_rect->size.height) {
            //compare_rect(src_rect, dst_rect);
            //const ei_point_t origine_src = src_rect->top_left;
            //const ei_point_t origine_dst = dst_rect->top_left;
            //copy_surface(source, origine_src, destination, origine_dst, dst_rect->size, alpha);

            return 1;
        } else if (src_rect->size.width == dst_rect->size.width && src_rect->size.height == dst_rect->size.height) {
            const ei_point_t origine_src = src_rect->top_left;
            const ei_point_t origine_dst = dst_rect->top_left;
            copy_surface(source, origine_src, destination, origine_dst, dst_rect->size, alpha);
            return 0;
        }

    }

}



void	ei_drawentry_text	(ei_surface_t		surface,
                              const ei_point_t*	where,
                              ei_const_string_t	text,
                              ei_font_t		font,
                              ei_color_t		color,
                              const ei_rect_t*	clipper ){//,ei_point_t pos_curseur){


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

    pointeur_surface = pointeur_surface + 4*(where->y * width_surface + where->x-2);
    //int32_t i = (where->y * width_surface + where->x;
    int i,j, i1, j1;
    int ir, ig, ib, ia;
    int r_s, g_s, b_s, r_d, g_d, b_d, a_s, a_d;

    ei_point_t pos_deb = clipper->top_left;
    ei_size_t dim_clip = clipper->size;

    //int xdeb_dest = pos_curseur.x;

    hw_surface_get_channel_indices(surf, &ir, &ig, &ib, &ia);
    int x_i = where->x;
    int y_i = where->y;
    ei_size_t taille_prin = hw_surface_get_size(root_window);
    for(j=0; j < size_surf.height; j++){
        pointeur_surf += 4*(size_surf.width);
        pointeur_surface += 4*(dim_clip.width);
        for(i=size_surf.width-1, i1 = dim_clip.width;i >=0  ;i--, i1--) {

            if(x_i+i1 >= 0 &&  y_i+j>=0 && x_i+i1 < taille_prin.width && y_i+j < taille_prin.height&&
               x_i+i1 >= pos_deb.x && x_i+i1 <= pos_deb.x + dim_clip.width && y_i+j >= pos_deb.y && y_i+j <= pos_deb.y + dim_clip.height) {

                //j1 = size_surf.height-j;

                r_s = pointeur_surf[ir];
                g_s = pointeur_surf[ig];
                b_s = pointeur_surf[ib];
                a_s = pointeur_surf[ia];

                pointeur_surface[ir] = (pointeur_surface[ir] * (255 - a_s) + r_s * a_s) / 255;
                pointeur_surface[ig] = (pointeur_surface[ig] * (255 - a_s) + g_s * a_s) / 255;
                pointeur_surface[ib] = (pointeur_surface[ib] * (255 - a_s) + b_s * a_s) / 255;
                //pointeur_surface[ia] = (pointeur_surface[ia]*(255 - a_s) + r_s*a_s)/255;
            }
            pointeur_surface -= 4;
            pointeur_surf -= 4;
            //free(couleur);
        }
        //pointeur_surf++;
        //pointeur_surface++;
        pointeur_surf += 4*width_surf;
        pointeur_surface = pointeur_surface + 4*(width_surface+ width_surf - dim_clip.width);
    }
    hw_surface_unlock(surf);
    hw_surface_free(surf);
    hw_surface_unlock(surface);

}
