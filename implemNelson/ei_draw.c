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
#include "ei_utils.h"

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



/*
int	ei_copy_surface		(ei_surface_t		destination,
                               const ei_rect_t*	dst_rect,
                               ei_surface_t		source,
                               const ei_rect_t*	src_rect,
                               bool			alpha){

    ei_size_t size_source = hw_surface_get_size( source);
    ei_size_t size_destination = hw_surface_get_size( destination);

    if (size_source.width != size_destination.width  || size_source.height != size_destination.height){
        if (src_rect == NULL && dst_rect == NULL ){
            return 1;
        }
        else if (src_rect->size.width != dst_rect->size.width  || src_rect->size.height != dst_rect->size.height){
            return 1;
        }
        else if (src_rect->size.width == dst_rect->size.width  && src_rect->size.height == dst_rect->size.height){
            const ei_point_t origine_src  = src_rect->top_left;
            const ei_point_t origine_dst = dst_rect->top_left;
            ei_size_t size = src_rect->size;
            copy_surface (source, origine_src,destination, origine_dst, size, alpha);
            return 0;
        }
    }

    else{
        if ( src_rect == NULL && dst_rect == NULL ){
            ei_rect_t rect_src =  hw_surface_get_rect(source);
            ei_rect_t rect_dst =  hw_surface_get_rect(destination);
            const ei_point_t src_origine  = rect_src.top_left;
            const ei_point_t dst_origine = rect_dst.top_left;
            ei_size_t size = rect_src.size;
            copy_surface (source, src_origine,destination, dst_origine, size, alpha);
            return 0;
        }
        else if (src_rect != NULL && dst_rect == NULL){
            const ei_point_t src_origine  = src_rect->top_left;
            ei_rect_t rect_dst = {hw_surface_get_rect(destination).top_left, src_rect->size};
            const ei_point_t dst_origine = rect_dst.top_left;
            ei_size_t size = src_rect->size;
            copy_surface (source, src_origine, destination, dst_origine, size, alpha);
            return 0;
        }
        else if (src_rect == NULL && dst_rect != NULL){
            return 1;
        }
        else if (src_rect != NULL && dst_rect != NULL){
            if(src_rect->size.width == dst_rect->size.width  && src_rect->size.height == dst_rect->size.height) {
                const ei_point_t origine_src  = src_rect->top_left;
                const ei_point_t origine_dst = dst_rect->top_left;
                ei_size_t size = src_rect->size;
                copy_surface (source, origine_src,destination, origine_dst, size, alpha);
                return 0;
            }
            else if (src_rect->size.width != dst_rect->size.width  || src_rect->size.height != dst_rect->size.height){
                return 1;
            }
        }
     }

}
*/


/*
void draw_img	(ei_surface_t		destination,
                               const ei_rect_t*	dst_rect,
                               ei_surface_t		source,
                               const ei_rect_t*	src_rect,
                               bool			alpha){

    ei_size_t size_source = hw_surface_get_size( source);
    ei_size_t size_destination = hw_surface_get_size( destination);

    if (size_source.width != size_destination.width  || size_source.height != size_destination.height){
        if (src_rect == NULL && dst_rect == NULL ){

        }
        else if (src_rect->size.width != dst_rect->size.width  || src_rect->size.height != dst_rect->size.height){

        }
        else if (src_rect->size.width == dst_rect->size.width  && src_rect->size.height == dst_rect->size.height){
            const ei_point_t origine_src  = src_rect->top_left;
            const ei_point_t origine_dst = dst_rect->top_left;
            ei_size_t size = src_rect->size;
            copy_surface (source, origine_src,destination, origine_dst, size, alpha);

        }
    }

    else{
        if ( src_rect == NULL && dst_rect == NULL ){
            ei_rect_t rect_src =  hw_surface_get_rect(source);
            ei_rect_t rect_dst =  hw_surface_get_rect(destination);
            const ei_point_t src_origine  = rect_src.top_left;
            const ei_point_t dst_origine = rect_dst.top_left;
            ei_size_t size = rect_src.size;
            copy_surface (source, src_origine,destination, dst_origine, size, alpha);

        }
        else if (src_rect != NULL && dst_rect == NULL){
            const ei_point_t src_origine  = src_rect->top_left;
            ei_rect_t rect_dst = {hw_surface_get_rect(destination).top_left, src_rect->size};
            const ei_point_t dst_origine = rect_dst.top_left;
            ei_size_t size = src_rect->size;
            copy_surface (source, src_origine, destination, dst_origine, size, alpha);

        }
        else if (src_rect == NULL && dst_rect != NULL){

        }
        else if (src_rect != NULL && dst_rect != NULL){
            if(src_rect->size.width == dst_rect->size.width  && src_rect->size.height == dst_rect->size.height) {
                const ei_point_t origine_src  = src_rect->top_left;
                const ei_point_t origine_dst = dst_rect->top_left;
                ei_size_t size = src_rect->size;
                copy_surface (source, origine_src,destination, origine_dst, size, alpha);
            }
            else if (src_rect->size.width != dst_rect->size.width  || src_rect->size.height != dst_rect->size.height){
            }
        }
    }

}
 */

void copy_surface (ei_surface_t source, const ei_point_t origine_src, ei_surface_t destination, const ei_point_t origine_dst, ei_size_t size, bool alpha){
    //on suppose que les deux sources sont de même taille "à partir des origines considérées" et que toute la surface est utilisée
    hw_surface_lock(source);
    hw_surface_lock(destination);


    ei_size_t size_src = hw_surface_get_size( source);
    ei_size_t size_dst = hw_surface_get_size( destination);
    //ei_size_t size_dst = hw_surface_get_size( source);

    hw_surface_set_origin(destination, origine_dst);
    uint8_t* ptr_dst = hw_surface_get_buffer(destination);


    hw_surface_set_origin(source, origine_src);
    uint8_t* ptr_src = hw_surface_get_buffer(source);


    int i;
    int j;
    int ir, ig, ib, ia;
    int r_s, g_s, b_s, r_d, g_d, b_d, a_s, a_d;

    if (alpha == false){a_s = 255;}


    hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);

    for(j=0; j < size.height; j++){

        for(i=0;i < size.width;i++) {

            r_s = ptr_src[ir];
            g_s = ptr_src[ig];
            b_s = ptr_src[ib];
            a_s = ptr_src[ia];


            ptr_dst[ir] = (ptr_dst[ir] * (255 - a_s) + r_s * a_s) / 255;
            ptr_dst[ig] = (ptr_dst[ig] * (255 - a_s) + g_s * a_s) / 255;
            ptr_dst[ib] = (ptr_dst[ib] * (255 - a_s) + b_s * a_s) / 255;

            ptr_dst += 4;
            ptr_src += 4;
        }




        ptr_dst = ptr_dst + 4*(size_dst.width - size.width );
        ptr_src = ptr_src + 4*(size_src.width - size.width );

    }

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