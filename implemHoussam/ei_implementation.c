//
// Created by ensimag on 5/6/24.
//

#include "ei_application.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widgetclass.h"

#include <ei_widget_attributes.h>
#include <locale.h>

#include "ei_implementation.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_utils.h"

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                             ei_surface_t		surface,
                                             ei_surface_t		pick_surface,
                                             ei_rect_t*		clipper) {
    if (widget->children_head != NULL) {
        widget->children_head->wclass->drawfunc(widget->children_head, surface, pick_surface, clipper);
    }

}



//ajout des fonctions par Nelson
ei_arc_t* arc(int32_t rayon, ei_point_t centre, double angle_debut, double angle_fin)
{
    ei_point_t* points = malloc(sizeof(ei_point_t));

    double delta = angle_fin - angle_debut ;

    int32_t taille = 0;

    ei_point_t point;

    while (angle_debut <= angle_fin)
    {
        taille++;

        points = realloc(points, taille * sizeof(ei_point_t));

        point.x = centre.x + rayon * cos(angle_debut);

        point.y = centre.y + rayon * sin(angle_debut);

        points[taille-1] = point;

        angle_debut = angle_debut + delta/10;
    }

    ei_arc_t* arcc = malloc(sizeof(ei_arc_t));
    arcc->taille = taille;
    arcc->points = points;

    return arcc;
}


ei_point_t* concatenation_tab(ei_point_t* tab1, int32_t taille1, ei_point_t* tab2, int32_t taille2)
{

    ei_point_t* tableau = malloc(sizeof(ei_point_t));
    int32_t i = 0;
    int32_t j = 0;

    while(i < taille1)
    {
        i++;
        tableau = realloc(tableau, i * sizeof(ei_point_t) );
        tableau[i-1] = tab1[i-1];
    }

    while(i < taille1 + taille2)
    {
        i++;
        tableau = realloc(tableau, i * sizeof(ei_point_t) );
        tableau[i-1] = tab2[j];
        j++;
    }

    return tableau;
}


ei_arc_t* rounded_frame(int32_t rayon, ei_rect_t rectangle)
{
    int	width = rectangle.size.width;
    int	height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;


    ei_point_t c1 = {x + rayon, y + rayon};
    ei_point_t* points_1 = arc(rayon, c1, -M_PI, -M_PI/2)->points;
    int32_t	taille1 = arc(rayon, c1, -M_PI, -M_PI/2)->taille;


    ei_point_t c2 = {x + width - rayon, y + rayon};
    ei_point_t* points_2 = arc(rayon, c2, -M_PI/2, -M_PI/4)->points;
    int32_t	taille2 = arc(rayon, c2, -M_PI/2, -M_PI/4)->taille;


    ei_point_t c3 = {x + width - rayon, y + rayon};
    ei_point_t* points_3 = arc(rayon, c3, -M_PI/4, 0)->points;
    int32_t	taille3 = arc(rayon, c3, -M_PI/4, 0)->taille;

    ei_point_t c4 = {x + width - rayon, y + height - rayon};
    ei_point_t* points_4 = arc(rayon, c4, -2*M_PI, -3*M_PI/2)->points;
    int32_t	taille4 = arc(rayon, c4, -2*M_PI, -3*M_PI/2)->taille;


    ei_point_t c5 = {x + rayon, y + height - rayon};
    ei_point_t* points_5 = arc(rayon, c5, -3*M_PI/2, -5*M_PI/4)->points;
    int32_t	taille5 = arc(rayon, c5, -3*M_PI/2, -5*M_PI/4)->taille;


    ei_point_t c6 = {x + rayon, y + height - rayon};
    ei_point_t* points_6 = arc(rayon, c6, -5*M_PI/4, -M_PI)->points;
    int32_t	taille6 = arc(rayon, c6, -5*M_PI/4, -M_PI)->taille;



    ei_point_t* points = concatenation_tab( points_1, taille1, points_2, taille2);
    int32_t taille = taille1 + taille2;


    points = concatenation_tab(points, taille, points_3, taille3);
    taille = taille + taille3;


    points = concatenation_tab(points, taille, points_4, taille4);
    taille = taille + taille4;


    points = concatenation_tab(points, taille, points_5, taille5);
    taille = taille + taille5;


    points = concatenation_tab(points, taille, points_6, taille6);
    taille = taille + taille6;


    ei_arc_t* arcc = malloc(sizeof(ei_arc_t));
    arcc->taille = taille;
    arcc->points = points;

    return arcc;

}


ei_arc_bg_t* rounded_frame_bg(int32_t rayon, ei_rect_t rectangle, int32_t h) {
    /*
     int	width = rectangle.size.width;
    int	height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;

    ei_arc_bg_t* arcc = malloc(sizeof(ei_arc_t));



    ei_point_t* triangle_up = malloc(3 * sizeof(ei_point_t));
    triangle_up[0] = rectangle.top_left;
    triangle_up[1]  = (ei_point_t) {x + width, y};
    triangle_up[2] = (ei_point_t) {x + width, y + height};
    int32_t	taille_up = 3;


    ei_point_t* triangle_down = malloc(3 * sizeof(ei_point_t));
    triangle_down[0] = rectangle.top_left;
    triangle_down[1] =(ei_point_t) {x, y + height};
    triangle_down[2] = triangle_up[2];
    int32_t	taille_down = 3;

    arcc->taille_up = taille_up;
    arcc->points_up = triangle_up;



    arcc->taille_down = taille_down;
    arcc->points_down = triangle_down;
     */
    int width = rectangle.size.width;
    int height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;

    ei_arc_bg_t *arcc = malloc(sizeof(ei_arc_t));


    ei_point_t c1 = {x + rayon, y + rayon};
    ei_point_t *points_1 = arc(rayon, c1, -M_PI, -M_PI / 2)->points;
    int32_t taille1 = arc(rayon, c1, -M_PI, -M_PI / 2)->taille;


    ei_point_t c2 = {x + width - rayon, y + rayon};
    ei_point_t *points_2 = arc(rayon, c2, -M_PI / 2, -M_PI / 4)->points;
    int32_t taille2 = arc(rayon, c2, -M_PI / 2, -M_PI / 4)->taille;
    /*
    ei_point_t pt3 = {x + width - h, y + h};
    ei_point_t *point3 = malloc(sizeof(ei_point_t));
    point3[0] = pt3;
    int32_t taille3 = 1;


    ei_point_t pt4 = {x + h, y + h};
    ei_point_t *point4 = malloc(sizeof(ei_point_t));
    point4[0] = pt4;
    int32_t taille4 = 1;
     */

    ei_point_t c5 = {x + rayon, y + height - rayon};
    ei_point_t *points_5 = arc(rayon, c5, -5 * M_PI / 4, -M_PI)->points;
    int32_t taille5 = arc(rayon, c5, -5 * M_PI / 4, -M_PI)->taille;

    ei_point_t *points_up = concatenation_tab(points_1, taille1, points_2, taille2);
    int32_t taille_up = taille1 + taille2;
/*
    points_up = concatenation_tab(points_up, taille_up, point3, taille3);
    taille_up = taille_up + taille3;

    points_up = concatenation_tab(points_up, taille_up, point4, taille4);
    taille_up = taille_up + taille4;
    */

    points_up = concatenation_tab(points_up, taille_up, points_5, taille5);
    taille_up = taille_up + taille5;

    arcc->taille_up = taille_up;
    arcc->points_up = points_up;


    ei_point_t c6 = {x + width - rayon, y + rayon};
    ei_point_t *points_6 = arc(rayon, c6, -M_PI / 4, 0)->points;
    int32_t taille6 = arc(rayon, c6, -M_PI / 4, 0)->taille;

    ei_point_t c7 = {x + width - rayon, y + height - rayon};
    ei_point_t *points_7 = arc(rayon, c7, -2 * M_PI, -3 * M_PI / 2)->points;
    int32_t taille7 = arc(rayon, c7, -2 * M_PI, -3 * M_PI / 2)->taille;


    ei_point_t c8 = {x + rayon, y + height - rayon};
    ei_point_t *points_8 = arc(rayon, c8, -3 * M_PI / 2, -5 * M_PI / 4)->points;
    int32_t taille8 = arc(rayon, c8, -3 * M_PI / 2, -5 * M_PI / 4)->taille;
/*
    ei_point_t *point9 = point4;
    int32_t taille9 = 1;

    ei_point_t *point10 = point3;
    int32_t taille10 = 1;
    */

    ei_point_t *points_down = concatenation_tab(points_6, taille6, points_7, taille7);
    int32_t taille_down = taille6 + taille7;

    points_down = concatenation_tab(points_down, taille_down, points_8, taille8);
    taille_down = taille_down + taille8;
/*
    points_down = concatenation_tab(points_down, taille_down, point9, taille9);
    taille_down = taille_down + taille9;

    points_down = concatenation_tab(points_down, taille_down, point10, taille10);
    taille_down = taille_down + taille10;
    */

    arcc->taille_down = taille_down;
    arcc->points_down = points_down;


    return arcc;
}


ei_arc_bg_t* triangle_frame_bg(ei_rect_t rectangle)
{
    int	width = rectangle.size.width;
    int	height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;

    ei_arc_bg_t* arcc = malloc(sizeof(ei_arc_t));



    ei_point_t* triangle_up = malloc(3 * sizeof(ei_point_t));
    triangle_up[0] = rectangle.top_left;
    triangle_up[1]  = (ei_point_t) {x + width, y};
    triangle_up[2] = (ei_point_t) {x + width, y + height};
    int32_t	taille_up = 3;


    ei_point_t* triangle_down = malloc(3 * sizeof(ei_point_t));
    triangle_down[0] = rectangle.top_left;
    triangle_down[1] =(ei_point_t) {x, y + height};
    triangle_down[2] = triangle_up[2];
    int32_t	taille_down = 3;

    arcc->taille_up = taille_up;
    arcc->points_up = triangle_up;



    arcc->taille_down = taille_down;
    arcc->points_down = triangle_down;



    return arcc;

}


ei_arc_t* rounded_top_level(int32_t rayon, ei_rect_t rectangle)
{
    int	width = rectangle.size.width;
    int	height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;


    ei_point_t c1 = {x + rayon, y + rayon};
    ei_point_t* points_1 = arc(rayon, c1, -M_PI, -M_PI/2)->points;
    int32_t	taille1 = arc(rayon, c1, -M_PI, -M_PI/2)->taille;


    ei_point_t c2 = {x + width - rayon, y + rayon};
    ei_point_t* points_2 = arc(rayon, c2, -M_PI/2, 0)->points;
    int32_t	taille2 = arc(rayon, c2, -M_PI/2, 0)->taille;


    ei_point_t pt3 = {x + width, y + height};
    ei_point_t* point3 = malloc(sizeof(ei_point_t));
    point3[0] = pt3;
    int32_t taille3 = 1;


    ei_point_t pt4 = {x, y + height};
    ei_point_t* point4 = malloc(sizeof(ei_point_t));
    point4[0] = pt4;
    int32_t taille4 = 1;


    ei_point_t* points = concatenation_tab( points_1, taille1, points_2, taille2);
    int32_t taille = taille1 + taille2;


    points = concatenation_tab(points, taille, point3, taille3);
    taille = taille + taille3;


    points = concatenation_tab(points, taille, point4, taille4);
    taille = taille + taille4;


    ei_arc_t* arcc = malloc(sizeof(ei_arc_t));
    arcc->taille = taille;
    arcc->points = points;

    return arcc;

}

//Cette fonction transforme un entier sur 32 bits en une couleur

ei_color_t* map_pick_id_to_color(ei_surface_t surface, uint32_t pick_id){
    ei_color_t* color = malloc(sizeof(ei_color_t));

    if (color == NULL){
        //Gerer l'échec de l'allocation de memoire
        fprintf(stderr, "Erreur lors de l'allocation de mémoire pour la couleur. \n");
        exit(EXIT_FAILURE);

    }
    int ir, ig, ib, ia;
    hw_surface_lock(surface);
    uint8_t* pixel_ptr = (uint8_t*)(&pick_id);
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

    //Extraire les composantes de couleur et les stocker dans la structure
    color->red = pixel_ptr[ir];  //Composante rouge
    color->green = pixel_ptr[ig];  //Composante verte
    color->blue = pixel_ptr[ib];  //Composante bleue
    //color->alpha = (pick_id >> 8*ia)& 0xFF;
    color->alpha =255;

    hw_surface_unlock(surface);
    return color;
}


ei_point_t*  place_text ( ei_widget_t widget, ei_const_string_t	text, const ei_font_t	font, ei_anchor_t text_anchor){

    ei_point_t* where = malloc(sizeof(ei_point_t));
    int width_text;
    int height_text;
    hw_text_compute_size(text, font, &width_text, &height_text);

    if (text_anchor == ei_anc_none || text_anchor == ei_anc_center){
        where->x = widget->screen_location.top_left.x + widget->screen_location.size.width/2 - width_text/2;
        where->y = widget->screen_location.top_left.y + widget->screen_location.size.height/2 - height_text/2;
    }
    else if (text_anchor == ei_anc_south){
        where->x = widget->screen_location.top_left.x + widget->screen_location.size.width/2 - width_text/2;
        where->y = widget->screen_location.top_left.y + widget->screen_location.size.height - height_text;
    }
    else if (text_anchor == ei_anc_north){
        where->x = widget->screen_location.top_left.x + widget->screen_location.size.width/2 - width_text/2;
        where->y = widget->screen_location.top_left.y ;
    }
    else if (text_anchor == ei_anc_east){
        where->x = widget->screen_location.top_left.x + widget->screen_location.size.width - width_text;
        where->y = widget->screen_location.top_left.y + widget->screen_location.size.height/2 - height_text/2;
    }
    else if (text_anchor == ei_anc_west){
        where->x = widget->screen_location.top_left.x;
        where->y = widget->screen_location.top_left.y + widget->screen_location.size.height/2 - height_text/2;
    }
    else if (text_anchor == ei_anc_southwest){
        where->x = widget->screen_location.top_left.x;
        where->y = widget->screen_location.top_left.y + widget->screen_location.size.height - height_text;
    }
    else if (text_anchor == ei_anc_southeast){
        where->x = widget->screen_location.top_left.x + widget->screen_location.size.width - width_text;
        where->y = widget->screen_location.top_left.y + widget->screen_location.size.height - height_text;
    }
    else if (text_anchor == ei_anc_northeast){
        where->x = widget->screen_location.top_left.x + widget->screen_location.size.width - width_text;
        where->y = widget->screen_location.top_left.y;
    }
    else if (text_anchor == ei_anc_northwest){
        where->x = widget->screen_location.top_left.x;
        where->y = widget->screen_location.top_left.y;
    }

    return where;

}


ei_rect_ptr_t  place_img (  ei_widget_t widget, ei_surface_t img, ei_rect_ptr_t img_rect, ei_anchor_t img_anchor){

    ei_rect_ptr_t where = malloc(sizeof(ei_rect_t)); //le rectangle sur la frame sur lequel on va dessiner l'image

    if (strcmp(widget->wclass->name, "frame") == 0){

        ei_impl_frame_t* new_widget = (ei_impl_frame_t*) widget;
        if (new_widget->img_rect->size.width <= widget->requested_size.width){
            where->size.width = img_rect->size.width;
        }
        if (new_widget->img_rect->size.height <= widget->requested_size.height){
            where->size.height = img_rect->size.height;
        }

        if (new_widget->img_rect->size.width > widget->requested_size.width){
            where->size.width = widget->requested_size.width;
        }
        if (new_widget->img_rect->size.height > widget->requested_size.height){
            where->size.height = widget->requested_size.height;
        }


    }

    else if (strcmp(widget->wclass->name, "button") == 0){

        ei_impl_button_t* new_widget = (ei_impl_button_t*) widget;
        //si la source est plus petite que la destination
        if (new_widget->img_rect->size.width <= widget->requested_size.width){
            where->size.width = img_rect->size.width;
        }
        if (new_widget->img_rect->size.height <= widget->requested_size.height){
            where->size.height = img_rect->size.height;
        }
        else{
            where->size = widget->requested_size;
        }
    }


    if (img_anchor == ei_anc_none || img_anchor == ei_anc_center){
        where->top_left.x = widget->screen_location.top_left.x +  widget->screen_location.size.width/2 - img_rect->size.width/2;
        where->top_left.y = widget->screen_location.top_left.y +  widget->screen_location.size.height/2 - img_rect->size.height/2;
    }

    else if (img_anchor == ei_anc_south){
        where->top_left.x = widget->screen_location.top_left.x +  widget->screen_location.size.width/2 - img_rect->size.width/2;
        where->top_left.y = widget->screen_location.top_left.y +  widget->screen_location.size.height - img_rect->size.height;
    }

    else if (img_anchor == ei_anc_north){
        where->top_left.x = widget->screen_location.top_left.x +  widget->screen_location.size.width/2 - img_rect->size.width/2;
        where->top_left.y = widget->screen_location.top_left.y;
    }

    else if (img_anchor == ei_anc_east){
        where->top_left.x = widget->screen_location.top_left.x +  widget->screen_location.size.width - img_rect->size.width;
        where->top_left.y = widget->screen_location.top_left.y +  widget->screen_location.size.height/2 - img_rect->size.height/2;
    }

    else if (img_anchor == ei_anc_west){
        where->top_left.x = widget->screen_location.top_left.x;
        where->top_left.y = widget->screen_location.top_left.y +  widget->screen_location.size.height/2 - img_rect->size.height/2;
    }

    else if (img_anchor == ei_anc_southwest){
        where->top_left.x = widget->screen_location.top_left.x;
        where->top_left.y = widget->screen_location.top_left.y +  widget->screen_location.size.height - img_rect->size.height;
    }

    else if (img_anchor == ei_anc_southeast){
        where->top_left.x = widget->screen_location.top_left.x +  widget->screen_location.size.width - img_rect->size.width;
        where->top_left.y = widget->screen_location.top_left.y +  widget->screen_location.size.height - img_rect->size.height;
    }

    else if (img_anchor == ei_anc_northeast){
        where->top_left.x = widget->screen_location.top_left.x +  widget->screen_location.size.width - img_rect->size.width;
        where->top_left.y = widget->screen_location.top_left.y;
    }

    else if (img_anchor == ei_anc_northwest){
        where->top_left.x = widget->screen_location.top_left.x;
        where->top_left.y = widget->screen_location.top_left.y;
    }

    where->top_left = ei_point_neg(where->top_left);

    return where;
}


void free_place_text( ei_point_t* point ){ free(point);}


void free_place_img( ei_rect_ptr_t rect ){ free(rect);}


void compare_rect(ei_widget_t widget, ei_rect_ptr_t source, ei_anchor_t img_anchor){

    if (img_anchor == ei_anc_northeast) {
        if (source->size.width > widget->requested_size.width) {
            source->top_left.x = source->size.width - widget->requested_size.width;
            source->size.width = widget->requested_size.width;
        }
        if (source->size.height > widget->requested_size.height) {
            source->size.height = widget->requested_size.height;
        }
    }

    if (img_anchor == ei_anc_northwest) {
        if (source->size.width > widget->requested_size.width) {
            source->size.width = widget->requested_size.width;
        }
        if (source->size.height > widget->requested_size.height) {
            source->size.height = widget->requested_size.height;
        }

    }

    if (img_anchor == ei_anc_southwest) {

        if (source->size.width > widget->requested_size.width) {
            //source->top_left.x = source->size.width - destination->size.width;
            source->size.width = widget->requested_size.width;
        }
        if (source->size.height > widget->requested_size.height) {
            source->top_left.y = source->size.height - widget->requested_size.height;
            source->size.height = widget->requested_size.height;
        }
    }

    if (img_anchor == ei_anc_southeast) {
        if (source->size.width > widget->requested_size.width) {
            source->top_left.x = source->size.width - widget->requested_size.width;
            source->size.width = widget->requested_size.width;
        }
        if (source->size.height > widget->requested_size.height) {
            source->top_left.y = source->size.height - widget->requested_size.height;
            source->size.height = widget->requested_size.height;
        }
    }

    if (img_anchor == ei_anc_north){

    }




    source->top_left = ei_point_neg(source->top_left);

}

bool bouton_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_impl_button_t* bouton = (ei_impl_button_t*) widget;
    if (event->type == ei_ev_mouse_buttondown) {
        bouton->relief = ei_relief_sunken;
        printf("\n jai appuyé \n");
        ei_app_invalidate_rect(&widget->screen_location);
        return false;
    }
    else if (event->type == ei_ev_mouse_buttonup) {
        bouton->relief = ei_relief_raised;
        printf("\njai relaché\n");
        ei_app_invalidate_rect(&widget->screen_location);
        return false;
    }
    return true;
}
