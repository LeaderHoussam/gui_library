//
// Created by ensimag on 5/6/24.
//
#include "ei_implementation.h"

#include <ei_placer.h>
#include <ei_utils.h>

#include "ei_application.h"

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
    ei_arc_t*  arc_1 = arc(rayon, c1, -M_PI, -M_PI/2);
    ei_point_t* points_1 = arc_1->points;
    int32_t	taille1 = arc_1->taille;


    ei_point_t c2 = {x + width - rayon, y + rayon};
    ei_arc_t*  arc_2 = arc(rayon, c2, -M_PI/2, -M_PI/4);
    ei_point_t* points_2 = arc_2->points;
    int32_t	taille2 = arc_2->taille;


    ei_point_t c3 = {x + width - rayon, y + rayon};
    ei_arc_t*  arc_3 = arc(rayon, c3, -M_PI/4, 0);
    ei_point_t* points_3 = arc_3->points;
    int32_t	taille3 = arc_3->taille;

    ei_point_t c4 = {x + width - rayon, y + height - rayon};
    ei_arc_t*  arc_4 = arc(rayon, c4, -2*M_PI, -3*M_PI/2);
    ei_point_t* points_4 = arc_4->points;
    int32_t	taille4 = arc_4->taille;


    ei_point_t c5 = {x + rayon, y + height - rayon};
    ei_arc_t*  arc_5 = arc(rayon, c5, -3*M_PI/2, -5*M_PI/4);
    ei_point_t* points_5 = arc_5->points;
    int32_t	taille5 = arc_5->taille;


    ei_point_t c6 = {x + rayon, y + height - rayon};
    ei_arc_t*  arc_6 = arc(rayon, c6, -5*M_PI/4, -M_PI);
    ei_point_t* points_6 = arc_6->points;
    int32_t	taille6 = arc_6->taille;


    ei_point_t* points_01 = concatenation_tab( points_1, taille1, points_2, taille2);
    int32_t taille01 = taille1 + taille2;

    ei_point_t* points_02 = concatenation_tab( points_3, taille3, points_4, taille4);
    int32_t taille02 = taille4 + taille4;

    ei_point_t* points_03 = concatenation_tab( points_5, taille5, points_6, taille6);
    int32_t taille03 = taille5 + taille6;


    ei_point_t* points_001 = concatenation_tab( points_01, taille01, points_02, taille02);
    int32_t taille001 = taille01 + taille02;

    ei_point_t* points = concatenation_tab( points_001, taille001, points_03, taille03);
    int32_t taille = taille001 + taille03;



    ei_arc_t* arcc = malloc(sizeof(ei_arc_t));
    arcc->taille = taille;
    arcc->points = points;


    free_arc(arc_1);
    free_arc(arc_2);
    free_arc(arc_3);
    free_arc(arc_4);
    free_arc(arc_5);
    free_arc(arc_6);
    free(points_01);
    free(points_02);
    free(points_03);
    free(points_001);

    return arcc;

}

ei_arc_bg_t* rounded_frame_bg(int32_t rayon, ei_rect_t rectangle) {

    int width = rectangle.size.width;
    int height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;

    ei_arc_bg_t *arcc = malloc(sizeof(ei_arc_bg_t));


    ei_point_t c1 = {x + rayon, y + rayon};
    ei_arc_t* arc_1 = arc(rayon, c1, -M_PI, -M_PI / 2);
    ei_point_t *points_1 = arc_1->points;
    int32_t taille1 = arc_1->taille;


    ei_point_t c2 = {x + width - rayon, y + rayon};
    ei_arc_t* arc_2 = arc(rayon, c2, -M_PI / 2, -M_PI / 4);
    ei_point_t *points_2 = arc_2->points;
    int32_t taille2 = arc_2->taille;

    ei_point_t c5 = {x + rayon, y + height - rayon};
    ei_arc_t* arc_5 = arc(rayon, c5, -5 * M_PI / 4, -M_PI);
    ei_point_t *points_5 = arc_5->points;
    int32_t taille5 = arc_5->taille;

    /*
    ei_point_t* concatenation_up = concatenation_tab(points_1, taille1, points_2, taille2);
    ei_point_t *points_up = concatenation_up;
    int32_t taille_up = taille1 + taille2;

    concatenation_up = concatenation_tab(points_up, taille_up, points_5, taille5);
    points_up = concatenation_up;
    taille_up = taille_up + taille5;
     */


    ei_point_t *points_up_01 = concatenation_tab(points_1, taille1, points_2, taille2);
    int32_t taille_up_01 = taille1 + taille2;


    ei_point_t *points_up = concatenation_tab(points_up_01, taille_up_01, points_5, taille5);
    int32_t  taille_up = taille_up_01 + taille5;

    arcc->taille_up = taille_up;
    arcc->points_up = points_up;


    ei_point_t c6 = {x + width - rayon, y + rayon};
    ei_arc_t* arc_6 = arc(rayon, c6, -M_PI / 4, 0);
    ei_point_t *points_6 = arc_6->points;
    int32_t taille6 = arc_6->taille;

    ei_point_t c7 = {x + width - rayon, y + height - rayon};
    ei_arc_t* arc_7 = arc(rayon, c7, -2 * M_PI, -3 * M_PI / 2);
    ei_point_t *points_7 = arc_7->points;
    int32_t taille7 = arc_7->taille;


    ei_point_t c8 = {x + rayon, y + height - rayon};
    ei_arc_t* arc_8 = arc(rayon, c8, -3 * M_PI / 2, -5 * M_PI / 4);
    ei_point_t *points_8 = arc_8->points;
    int32_t taille8 = arc_8->taille;

    /*
    ei_point_t* concatenation_down = concatenation_tab(points_6, taille6, points_7, taille7);
    ei_point_t *points_down = concatenation_down;
    int32_t taille_down = taille6 + taille7;

    concatenation_down = concatenation_tab(points_down, taille_down, points_8, taille8);
    points_down = concatenation_down;
    taille_down = taille_down + taille8;
     */

    ei_point_t *points_down_01 = concatenation_tab(points_6, taille6, points_7, taille7);
    int32_t taille_down_01 = taille6 + taille7;

    ei_point_t *points_down = concatenation_tab(points_down_01, taille_down_01, points_8, taille8);
    int32_t taille_down = taille_down_01 + taille8;


    arcc->taille_down = taille_down;
    arcc->points_down = points_down;


    free_arc(arc_1);
    free_arc(arc_2);
    free_arc(arc_5);
    free_arc(arc_6);
    free_arc(arc_7);
    free_arc(arc_8);
    free(points_up_01);
    free(points_down_01);
    //free(concatenation_up);
    //free(concatenation_down);
    return arcc;
}


ei_arc_bg_t* triangle_frame_bg(ei_rect_t rectangle)
{
    int	width = rectangle.size.width;
    int	height = rectangle.size.height;
    int x = rectangle.top_left.x;
    int y = rectangle.top_left.y;

    ei_arc_bg_t* arcc = malloc(sizeof(ei_arc_bg_t));



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
    ei_arc_t* arc_1 = arc(rayon, c1, -M_PI, -M_PI/2);
    ei_point_t* points_1 = arc_1->points;
    int32_t	taille1 = arc_1->taille;


    ei_point_t c2 = {x + width - rayon, y + rayon};
    ei_arc_t* arc_2 = arc(rayon, c2, -M_PI/2, 0);
    ei_point_t* points_2 = arc_2->points;
    int32_t	taille2 = arc_2->taille;


    ei_point_t pt3 = {x + width, y + height};
    ei_point_t* point3 = malloc(sizeof(ei_point_t));
    point3[0] = pt3;
    int32_t taille3 = 1;


    ei_point_t pt4 = {x, y + height};
    ei_point_t* point4 = malloc(sizeof(ei_point_t));
    point4[0] = pt4;
    int32_t taille4 = 1;

    /*
    ei_point_t* concatenation = concatenation_tab( points_1, taille1, points_2, taille2);
    ei_point_t* points = concatenation;
    int32_t taille = taille1 + taille2;

    concatenation = concatenation_tab(points, taille, point3, taille3);
    points = concatenation;
    taille = taille + taille3;

    concatenation = concatenation_tab(points, taille, point4, taille4);
    points = concatenation;
    taille = taille + taille4;
     */

    ei_point_t* points_01 = concatenation_tab( points_1, taille1, points_2, taille2);
    int32_t taille01 = taille1 + taille2;


    ei_point_t* points_02 = concatenation_tab(point3, taille3, point4, taille4);
    int32_t taille02 = taille3 + taille4;


    ei_point_t* points = concatenation_tab(points_01, taille01, points_02, taille02);
    int32_t taille = taille01 + taille02;


    ei_arc_t* arcc = malloc(sizeof(ei_arc_t));
    arcc->taille = taille;
    arcc->points = points;

    free_arc(arc_1);
    free_arc(arc_2);
    free(point3);
    free(point4);
    free(points_01);
    free(points_02);
    //free(concatenation);

    return arcc;

}

void free_arc(ei_arc_t* arc){
    free(arc->points);
    free(arc);
};

void free_arc_bg(ei_arc_bg_t* arc_bg){
    free(arc_bg->points_down);
    free(arc_bg->points_up);
    free(arc_bg);
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
    int x = widget->content_rect->top_left.x;
    int y = widget->content_rect->top_left.y;
    int w = widget->content_rect->size.width;
    int h = widget->content_rect->size.height;

    if (text_anchor == ei_anc_none || text_anchor == ei_anc_center){
        //where->x = widget->screen_location.top_left.x + widget->screen_location.size.width/2 - width_text/2;
        //where->y = widget->screen_location.top_left.y + widget->screen_location.size.height/2 - height_text/2;

        where->x = x + w/2 - width_text/2;
        where->y = y + h/2 - height_text/2;
    }
    else if (text_anchor == ei_anc_south){
        //where->x = widget->screen_location.top_left.x + widget->screen_location.size.width/2 - width_text/2;
        //where->y = widget->screen_location.top_left.y + widget->screen_location.size.height - height_text;

        where->x = x + w/2 - width_text/2;
        where->y = y + h - height_text;
    }
    else if (text_anchor == ei_anc_north){
        //where->x = widget->screen_location.top_left.x + widget->screen_location.size.width/2 - width_text/2;
        //where->y = widget->screen_location.top_left.y ;

        where->x = x + w/2 - width_text/2;
        where->y = y ;
    }
    else if (text_anchor == ei_anc_east){
        //where->x = widget->screen_location.top_left.x + widget->screen_location.size.width - width_text;
        //where->y = widget->screen_location.top_left.y + widget->screen_location.size.height/2 - height_text/2;

        where->x = x + w - width_text;
        where->y = y + h/2 - height_text/2;
    }
    else if (text_anchor == ei_anc_west){
        //where->x = widget->screen_location.top_left.x;
        //where->y = widget->screen_location.top_left.y + widget->screen_location.size.height/2 - height_text/2;

        where->x = x;
        where->y = y + h/2 - height_text/2;
    }
    else if (text_anchor == ei_anc_southwest){
        //where->x = widget->screen_location.top_left.x;
        // where->y = widget->screen_location.top_left.y + widget->screen_location.size.height - height_text;

        where->x = x;
        where->y = y + h - height_text;
    }
    else if (text_anchor == ei_anc_southeast){
        //where->x = widget->screen_location.top_left.x + widget->screen_location.size.width - width_text;
        //where->y = widget->screen_location.top_left.y + widget->screen_location.size.height - height_text;

        where->x = x + w - width_text;
        where->y = y + h - height_text;
    }
    else if (text_anchor == ei_anc_northeast){
        //where->x = widget->screen_location.top_left.x + widget->screen_location.size.width - width_text;
        //where->y = widget->screen_location.top_left.y;

        where->x = x + w - width_text;
        where->y = y;
    }
    else if (text_anchor == ei_anc_northwest){
        // where->x = widget->screen_location.top_left.x;
        //where->y = widget->screen_location.top_left.y;

        where->x = x;
        where->y = y;
    }


    return where;

}

//void state_button(ei_widget_t widget, ei_relief_t relief ) {
/*
bool bouton_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_impl_button_t* bouton = (ei_impl_button_t*) widget;
    if (event->type == ei_ev_mouse_buttondown) {
        bouton->relief = ei_relief_sunken;
        printf("\n jai appuyé \n");
        ei_app_invalidate_rect(&widget->screen_location);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", bouton_handler_1, widget);
        ei_bind(ei_ev_mouse_move, NULL, "all", bouton_handler_1, widget);

    }
    return false;
}
bool bouton_handler_1(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_widget_t widget_1 = (ei_widget_t) user_param;
    ei_impl_button_t* bouton = (ei_impl_button_t*) user_param;
    ei_rect_t pos_b = widget_1->screen_location;
    ei_point_t pos_souri = event->param.mouse.where;
    if (event->type == ei_ev_mouse_buttonup) {
        bouton->relief = ei_relief_raised;
        printf("\njai relaché\n");
        ei_app_invalidate_rect(&widget->screen_location);
        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", bouton_handler_1, user_param);
        ei_unbind(ei_ev_mouse_move, NULL, "all", bouton_handler_1, user_param);
    }
    else if(event->type == ei_ev_mouse_move && !(pos_souri.x >= pos_b.top_left.x
                                                 && pos_souri.x <= (pos_b.top_left.x+ pos_b.size.width) &&
                                                 pos_souri.y >= pos_b.top_left.y && pos_souri.y <= (pos_b.top_left.y + pos_b.size.height))) {

        bouton->relief = ei_relief_raised;
        printf("\njai relaché\n");
        ei_app_invalidate_rect(&widget->screen_location);
        //
    }
    else {
        bouton->relief = ei_relief_sunken;
        printf("\njai relaché\n");
        ei_app_invalidate_rect(&widget->screen_location);

    }
    return false;

}
bool toplevel_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_point_t pos_souri = event->param.mouse.where;
    ei_point_t coin_gauche = widget->screen_location.top_left;
    if(event->type == ei_ev_mouse_buttondown && (pos_souri.x >= coin_gauche.x+3) &&
       (pos_souri.x <= coin_gauche.x+17) && (pos_souri.y <= coin_gauche.y+17)  &&
       (pos_souri.y >= coin_gauche.y+3)) {
        printf("\n\nla croix rouge marche\n\n");
        ei_widget_destroy(widget);
    }
    return false;
}

bool toplevel_handler_1(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_point_t pos_souri = event->param.mouse.where;
    ei_point_t coin_gauche = widget->screen_location.top_left;
    ei_size_t taille = widget->screen_location.size;
    ei_impl_toplevel_t* top_level = (ei_impl_toplevel_t*) widget;
    ei_point_t pos_carre;
    pos_carre.x = widget->screen_location.top_left.x + widget->screen_location.size.width+ 2*top_level->border_width -10;
    pos_carre.y = widget->screen_location.top_left.y + widget->screen_location.size.height + 10 + top_level->border_width;


    if(event->type == ei_ev_mouse_buttondown && (pos_souri.x >= coin_gauche.x) &&
       (pos_souri.x <= coin_gauche.x+ taille.width) && (pos_souri.y <= coin_gauche.y+20)  &&
       (pos_souri.y >= coin_gauche.y)) {
        move_widget_to_end(widget);
        ei_app_invalidate_rect(&widget->screen_location);
        // à essayer quand j'aurai fait un_bind
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", toplevel_handler_2, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", toplevel_handler_2, NULL);


    }
    else if(event->type == ei_ev_mouse_buttondown && (pos_souri.x >= pos_carre.x) &&
            (pos_souri.x <= pos_carre.x+ 10) && (pos_souri.y <= pos_carre.y+10)  &&
            (pos_souri.y >= pos_carre.y)) {
        if(top_level->resizable != ei_axis_none) {
            ei_bind(ei_ev_mouse_buttonup, NULL, "all", toplevel_redimension, NULL);
            ei_bind(ei_ev_mouse_move, NULL, "all", toplevel_redimension, NULL);
        }

    }

    pos_mouse = pos_souri;
    return false;
}

bool toplevel_handler_2(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {

    int new_x, new_y;
    ei_event_t event_int;
    ei_point_t pos_int;

    //hw_event_wait_next(&event_int);
    if(event->type == ei_ev_mouse_move) {
        pos_int = event->param.mouse.where;
        new_x =  widget->screen_location.top_left.x + pos_int.x - pos_mouse.x;

        new_y =  widget->screen_location.top_left.y + pos_int.y - pos_mouse.y;
        ei_place_xy(widget, new_x, new_y);
        pos_mouse = pos_int;
    }
    else if(event->type == ei_ev_mouse_buttonup) {
        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", toplevel_handler_2, NULL);
        ei_unbind(ei_ev_mouse_move, NULL, "all", toplevel_handler_2, NULL);
    }
    return true;
}


bool toplevel_redimension(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {


    ei_impl_toplevel_t* top_level = (ei_impl_toplevel_t*) widget;
    int new_x, new_y;
    int new_w, new_h;

    ei_point_t pos_int;

    if(event->type == ei_ev_mouse_move) {
        pos_int = event->param.mouse.where;
        //new_x =  top_left.x + pos_int.x - pos_mouse.x;
        new_x =  pos_int.x - pos_mouse.x;

        //new_y =  top_left.y + pos_int.y - pos_mouse.y;
        new_y =pos_int.y - pos_mouse.y;


        new_w = widget->screen_location.size.width + new_x;
        new_h = widget->screen_location.size.height + new_y;
        int w = max(new_w,top_level->min_size->width);
        int h = max(new_h,top_level->min_size->height);
        if(top_level->resizable == ei_axis_both) {
            ei_place_wh(widget, w, h);
        }
        else if(top_level->resizable == ei_axis_x) {

            ei_place(widget,NULL,NULL, NULL, &w, NULL,NULL,NULL,NULL,NULL);
        }
        else {

            ei_place(widget,NULL,NULL, NULL, NULL, &h,NULL,NULL,NULL,NULL);
        }

        pos_mouse = pos_int;
    }
    else if(event->type == ei_ev_mouse_buttonup) {
        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", toplevel_redimension, NULL);
        ei_unbind(ei_ev_mouse_move, NULL, "all", toplevel_redimension, NULL);
    }
    return true;
}
*/



/*

bool toplevel_redimension(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_impl_toplevel_t* top_level = (ei_impl_toplevel_t*) widget;
    ei_point_t pos_souri = event->param.mouse.where;
    ei_point_t pos_carre;
    pos_carre.x = widget->screen_location.top_left.x + widget->screen_location.size.width+ 2*top_level->border_width -10;
    pos_carre.y = widget->screen_location.top_left.y + widget->screen_location.size.height + 10 + top_level->border_width;
    ei_point_t top_left = widget->screen_location.top_left;
    ei_impl_placeur_t* geo_par = (ei_impl_placeur_t*)widget->geom_params;
    int new_x, new_y;
    int new_w, new_h;
    ei_event_t event_int;
    ei_point_t pos_int;
    if(event->type == ei_ev_mouse_buttondown && (pos_souri.x >= pos_carre.x) &&
        (pos_souri.x <= pos_carre.x+ 10) && (pos_souri.y <= pos_carre.y+10)  &&
        (pos_souri.y >= pos_carre.y)) {
        while(true) {
            hw_event_wait_next(&event_int);
            if(event_int.type == ei_ev_mouse_move) {
                pos_int = event_int.param.mouse.where;
                new_x =  pos_int.x - pos_souri.x;
                new_y =  pos_int.y - pos_souri.y;

                //new_x =  (geo_par!= NULL) ? (geo_par->x+ pos_int.x - pos_souri.x): pos_int.x - (pos_souri.x-widget->screen_location.top_left.x);
                //new_y =  (geo_par!= NULL) ? (geo_par->y+ pos_int.y - pos_souri.y): pos_int.y - (pos_souri.x-widget->screen_location.top_left.y);
                new_w = widget->screen_location.size.width + new_x;
                new_h = widget->screen_location.size.height + new_y;
                int w = max(new_w,top_level->min_size->width);
                int h = max(new_h,top_level->min_size->height);
                if(top_level->resizable == ei_axis_both) {
                    ei_place_wh(widget, w, h);
                }
                else if(top_level->resizable == ei_axis_x) {
                    ei_place(widget,NULL,NULL, NULL, &w, NULL,NULL,NULL,NULL,NULL);
                }
                else {
                    ei_place(widget,NULL,NULL, NULL, NULL, &h,NULL,NULL,NULL,NULL);
                }

                //(*(root_widget->wclass->drawfunc))(root_widget, root_window, surface_arriere, root_widget->content_rect);
                //hw_surface_update_rects(root_window, NULL);

                (*(root_widget->wclass->drawfunc))(root_widget, root_window, surface_arriere, clipper_final);
                hw_surface_update_rects(root_window, surfaces_mise_a_jour);
                surfaces_mise_a_jour = NULL;
                pos_souri = pos_int;
            }
            else if(event_int.type == ei_ev_mouse_buttonup) {
                break;
            }
        }

    }
    return false;
}
*/

ei_rect_ptr_t  place_img (  ei_widget_t widget, ei_surface_t img, ei_rect_ptr_t img_rect, ei_anchor_t img_anchor){
    //agit sur la partie du widget sur lequel on doit dessiner l'image
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

        if (new_widget->img_rect->size.width > widget->requested_size.width){
            where->size.width = widget->requested_size.width;
        }
        if (new_widget->img_rect->size.height > widget->requested_size.height){
            where->size.height = widget->requested_size.height;
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

void free_widget(ei_widget_t widget){
    free(widget->geom_params);
    free(widget->pick_color);
    if (strcmp(widget->wclass->name , "toplevel") == 0){
        free(widget->content_rect);
        ei_impl_toplevel_t* top = (ei_impl_toplevel_t*) widget;
        free(top->min_size);
        free(top);
        return;
    }

    if (strcmp(widget->wclass->name , "frame") == 0){
        ei_impl_frame_t * frame = (ei_impl_frame_t *) widget;
        free(frame->img);
        if (frame->text != NULL) {
            free(frame->text);
            frame->text = NULL;
        }
        free(frame);
        return;
    }
    free(widget);
}


// Fonction pour supprimer un widget
void delete_widget(ei_widget_t widget) {
    if (widget == NULL || widget->parent == NULL) {
        return;
    }

    ei_widget_t parent = widget->parent;
    ei_widget_t current = parent->children_head;
    ei_widget_t prev = NULL;

    // Parcourir les enfants du parent pour trouver le widget à supprimer
    while (current != NULL) {
        if (current == widget) {
            // Trouvé le widget à supprimer
            if (prev == NULL) {
                // Widget à supprimer est le premier enfant
                parent->children_head = current->next_sibling;
            } else {
                // Widget à supprimer n'est pas le premier enfant
                prev->next_sibling = current->next_sibling;
            }

            if (current->next_sibling == NULL) {
                // Widget à supprimer est le dernier enfant
                parent->children_tail = prev;
            }

            // Libérer la mémoire du widget
/*
            ei_event_binding* event_bind;
            event_bind = EVENT_BINDINGS;

            while (event_bind->next != NULL){
                if (event_bind->widget == widget){
                    ei_unbind(event_bind->event_type, widget, event_bind->tag, event_bind->callback, event_bind->user_param);
                    event_bind = EVENT_BINDINGS;
                } else {
                    event_bind = event_bind->next;
                }
            }*/
            free_widget(widget);
            return;
        }
        prev = current;
        current = current->next_sibling;
    }

    printf("Widget not found in the list of children.\n");
}

// Fonction pour ajouter un widget à la fin des enfants de son parent
/*
void append_widget(ei_widget_t parent, ei_widget_t widget) {
    if (parent == NULL || widget == NULL) {
        return;
    }

    widget->parent = parent;
    widget->next_sibling = NULL;

    if (parent->children_head == NULL) {
        // Si le parent n'a pas encore d'enfants
        parent->children_head = widget;
        parent->children_tail = widget;
    } else {
        // Ajouter à la fin
        parent->children_tail->next_sibling = widget;
        parent->children_tail = widget;
    }
}
*/

/*
// Fonction pour déplacer un widget à la fin des enfants de son parent
void move_widget_to_end(ei_widget_t widget) {
    if (widget == NULL || widget->parent == NULL) {
        return;
    }
    ei_widget_t parent = widget->parent;
    ei_widget_t parent_tete = widget->children_head;

        ei_widget_t anc_tail = parent->children_tail;
        ei_widget_t avant_widget = parent_tete;
        while(parent_tete != widget && parent_tete != NULL) {
            avant_widget = parent_tete;
            parent_tete = parent_tete->next_sibling;
        }
        parent->children_tail = widget;
        avant_widget->next_sibling = widget->next_sibling;
        widget->next_sibling = NULL;
        anc_tail->next_sibling = widget;

}
*/

void move_widget_to_end(ei_widget_t widget) {
    if (widget == NULL || widget->parent == NULL) {
        return;
    }

    ei_widget_t parent = widget->parent;

    // Si le widget est déjà le dernier enfant, rien à faire
    if (parent->children_tail == widget) {
        return;
    }

    // Retirer le widget de sa position actuelle
    ei_widget_t current = parent->children_head;
    ei_widget_t previous = NULL;

    while (current != NULL && current != widget) {
        previous = current;
        current = current->next_sibling;
    }

    // Si le widget est trouvé
    if (current == widget) {
        // Si le widget est le premier enfant
        if (previous == NULL) {
            parent->children_head = widget->next_sibling;
        } else {
            previous->next_sibling = widget->next_sibling;
        }

        // Si le widget est le dernier enfant
        if (widget->next_sibling == NULL) {
            parent->children_tail = previous;
        }

        // Ajouter le widget à la fin de la liste des enfants
        if (parent->children_tail != NULL) {
            parent->children_tail->next_sibling = widget;
        }
        parent->children_tail = widget;
        widget->next_sibling = NULL;

        // Si le parent n'avait pas d'enfant (cas rare mais à considérer)
        if (parent->children_head == NULL) {
            parent->children_head = widget;
        }
    }
}


bool compare_widget_class_name_and_tag(ei_widgetclass_name_t name, char *tag) {
    size_t tag_length = strlen(tag);
    return strncmp(name, tag, tag_length) == 0;
}
