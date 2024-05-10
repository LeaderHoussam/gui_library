//
// Created by traorefv on 30/04/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "ei_widgetclass.h"

#include <ei_widget_attributes.h>
#include <locale.h>

#include "ei_implementation.h"
#include "ei_draw.h"

//extern ei_widgetclass_t* liste_des_classe =calloc(sizeof(ei_widgetclass_t), 1);
ei_widgetclass_t* liste_des_classe = NULL;
// fonction d'allocation mémoire pour une classe frame
ei_widget_t allocfunc_frame() {
    ei_impl_frame_t* espace_pour_frame = calloc(sizeof(ei_impl_frame_t),1);
    ei_widget_t espace = (ei_widget_t) espace_pour_frame;
    return espace;

}
// cette fonction va supprimer toute les mémoire allouer par des instance de la classe classe frame
// mais pas la classe frame elle même
// par exemple, frame va avoir un champ relief,
// un champ police (ei_font_t) ...
// il faut supprimer ces champs
void releasefunc_frame(ei_widget_t  widget) {
    ei_impl_frame_t* la_frame = (ei_impl_frame_t *) widget;
    free((void*)(la_frame->text));
    free((void*)(la_frame->text_font));
    free((void*)(la_frame->img));
    free((void*)(la_frame->img_rect));
    // il faudra ajouter ici la libérationi du paramètre image
    // c'est fait
}
void drawfunc_frame(ei_widget_t		widget,
                    ei_surface_t		surface,
                    ei_surface_t		pick_surface,
                    ei_rect_t*		clipper) {
    // cette fonction est chargée de dessiner sur notre frame:
    // on y reviendra

    hw_surface_lock(surface);
    //hw_surface_lock(pick_surface);
    ei_point_t pos_debut = widget->content_rect->top_left;
    //if faut verifier ici que les coordonnées sont bien dans ceux du parent
    // mais on le fait pas car notre geometrymanager runfunc s'en charge.
    // après relecture du poly, j'ai finalement compris qu'il faut le faire ici et non
    // dans le gestionnaire de geometry, car lui il n'est pas sensé modifier les dimensions
    // si j'ai bien compris
    ei_size_t dimension = widget->screen_location.size;
    //ei_size_t dim_contenant = widget->content_rect->size;
    //ei_point_t pos_1 = {m};
    ei_point_t debut_surface[4] = {pos_debut, {pos_debut.x + dimension.width,pos_debut.y}, {pos_debut.x + dimension.width,pos_debut.y + dimension.height }, {pos_debut.x ,pos_debut.y + dimension.height} };
    //ei_point_t debut_surface[4] = {{0,0}, {0,600}, {600,600 }, {600, 0} };

    // transformation du widget en frame
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    ei_draw_polygon(surface,debut_surface, 4, frame->color, clipper);

    // on doit dessiner les enfants dans le content_rect du widget
    ei_impl_widget_draw_children(widget, surface, pick_surface, widget->content_rect);

    // je pense il faut revoir ça après pour savoir si sa geometrie a changé avant de dessiner
    if (widget->next_sibling != NULL) {
        widget->next_sibling->wclass->drawfunc(widget->next_sibling, surface, pick_surface, clipper);
    }
    /*if( widget->parent != NULL && widget->parent->children_tail != NULL ) {
        widget->parent->children_tail->wclass->drawfunc(widget->parent->children_head, surface, pick_surface, clipper);
    }*/

    hw_surface_unlock(surface);
    //hw_surface_unlock(pick_surface);
    // ei_linked_rect_t* clipp = (ei_linked_rect_t*) clipper;


    printf("\nAH AH, fonction draw_frame appélee");

}

void setdefaultsfunc_frame(ei_widget_t widget) {
    // fonction chargée d'initialisée les valeurs par défauts d'une frame
    // hum hum, à y revenir après relecture du poly

    //widget = (ei_widget_t) widget;

    // les autres parametres non commun aux autres:
    // on fait d'abord un transcriptage avant de les utiliser
    ei_impl_frame_t* la_frame = (ei_impl_frame_t *) widget;

    la_frame->color = ei_default_background_color;
    la_frame->border_width = 0;
    la_frame->relief = ei_relief_none;
    la_frame->text = NULL;
    la_frame->text_font = ei_default_font;
    la_frame->text_color = ei_font_default_color;
    la_frame->text_anchor = ei_anc_center;
    la_frame->img = NULL;
    la_frame->img_rect = NULL;
    la_frame->img_anchor = ei_anc_center;


}

void geomnotifyfunc_frame(ei_widget_t widget) {

    // on actualise le screen location du widget
    //widget->screen_location = *ei_widget_get_screen_location(widget);
    // cette fonction fait savoir si y'a eu une modification
    // je ne la comprend pas encore, on lagarde vide
}
// création de la classe frame
ei_widgetclass_t* init_frame_classe () {
    ei_widgetclass_t* classe_frame = calloc(sizeof(ei_widgetclass_t), 1);
    strcpy(classe_frame->name, "frame");
    classe_frame->allocfunc = &(allocfunc_frame);
    classe_frame->drawfunc = &(drawfunc_frame);
    classe_frame->releasefunc = releasefunc_frame;
    classe_frame->setdefaultsfunc = setdefaultsfunc_frame;
    classe_frame->geomnotifyfunc = geomnotifyfunc_frame;
    classe_frame->next = NULL;
    return classe_frame;
}



// fonction d'allocation mémoire pour une classe button

ei_widget_t allocfunc_button() {
    ei_impl_button_t* espace_pour_button = calloc(sizeof(ei_impl_button_t),1);
    ei_widget_t espace = (ei_widget_t) espace_pour_button;
    return espace;

}
// cette fonction va supprimer toute les mémoire allouer par des instance de la classe classe button
// mais pas la classe button elle même
// par exemple, button va avoir un champ relief,
// un champ police (ei_font_t) ...
// il faut supprimer ces champs
void releasefunc_button(ei_widget_t  widget) {
    ei_impl_button_t* le_button = (ei_impl_button_t *) widget;
    free((void*)(le_button->text));
    free((void*)(le_button->text_font));
    free((void*)(le_button->img));
    free((void*)(le_button->img_rect));
    // il faudra ajouter ici la libérationi du paramètre image
    // c'est fait
}

void drawfunc_button(ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper) {
    // cette fonction est chargée de dessiner sur notre button:
    // on y reviendra

    hw_surface_lock(surface);

    int32_t rayon = ((ei_impl_button_t*)widget)->corner_radius;

    ei_point_t top_left_bg = widget->screen_location.top_left;
    ei_size_t size_bg = widget->screen_location.size;
    ei_rect_t rectangle_bg = widget->screen_location;

    //ei_point_t top_left = {rayon/5, rayon/5};
    //ei_size_t size = {size_bg.width - 2*top_left.x, size_bg.height - 2*top_left.y};

    ei_point_t top_left = {top_left_bg.x + rayon/5, top_left_bg.y + rayon/5};
    ei_size_t size = {size_bg.width - 2*rayon/5, size_bg.height - 2*rayon/5};



    ei_rect_t rectangle = {top_left, size};


    int32_t h = size_bg.height / 2;


    // transformation du widget en button
    ei_impl_button_t* button = (ei_impl_button_t*) widget;

    ei_color_t couleur = button->color;
    ei_color_t couleur_fonce = {couleur.red*0.7, couleur.green*0.7, couleur.blue*0.7, couleur.alpha};
    ei_color_t couleur_clair = {couleur.red*1.3, couleur.green*1.3, couleur.blue*1.3, couleur.alpha};

    ei_point_t* debut_surface_up = rounded_frame_bg(rayon, rectangle_bg, h)->points_up;
    int32_t	taille_up = rounded_frame_bg(rayon, rectangle_bg, h)->taille_up;

    ei_point_t* debut_surface_down = rounded_frame_bg(rayon, rectangle_bg, h)->points_down;
    int32_t	taille_down = rounded_frame_bg(rayon, rectangle_bg, h)->taille_down;

    ei_point_t* debut_surface = rounded_frame(rayon, rectangle)->points;
    int32_t	taille = rounded_frame(rayon, rectangle_bg)->taille;


    ei_draw_polygon(surface,debut_surface_up, taille_up, couleur_fonce, clipper);
    ei_draw_polygon(surface,debut_surface_down, taille_down, couleur_clair, clipper);
    ei_draw_polygon(surface,debut_surface, taille, couleur, clipper);

    ei_impl_widget_draw_children(widget, surface, pick_surface, clipper);

    if (widget->next_sibling != NULL) {
        widget->next_sibling->wclass->drawfunc(widget->next_sibling, surface, pick_surface, clipper);
    }

    hw_surface_unlock(surface);
    //hw_surface_unlock(pick_surface);
    // ei_linked_rect_t* clipp = (ei_linked_rect_t*) clipper;


    printf("\nAH AH, fonction draw_button appélee");



}


void setdefaultsfunc_button(ei_widget_t widget) {
    // fonction chargée d'initialisée les valeurs par défauts d'un button
    // hum hum, à y revenir après relecture du poly


    // les autres parametres non commun aux autres:
    // on fait d'abord un transcriptage avant de les utiliser
    ei_impl_button_t* le_button = (ei_impl_button_t *) widget;

    le_button->color = ei_default_background_color;
    le_button->border_width = 0;
    le_button->corner_radius = 0;
    le_button->relief = ei_relief_none;
    le_button->text = NULL;
    le_button->text_font = ei_default_font;
    le_button->text_color = ei_font_default_color;
    le_button->text_anchor = ei_anc_center;
    le_button->img = NULL;
    le_button->img_rect = NULL;
    le_button->img_anchor = ei_anc_center;


}

void geomnotifyfunc_button(ei_widget_t widget) {

    // on actualise le screen location du widget
    //widget->screen_location = *ei_widget_get_screen_location(widget);
    // cette fonction fait savoir si y'a eu une modification
    // je ne la comprend pas encore, on lagarde vide
}

// création de la classe button
ei_widgetclass_t* init_button_classe () {
    ei_widgetclass_t* classe_button = calloc(sizeof(ei_widgetclass_t), 1);
    strcpy(classe_button->name, "button");
    classe_button->allocfunc = &(allocfunc_button);
    classe_button->drawfunc = &(drawfunc_button);
    classe_button->releasefunc = releasefunc_button;
    classe_button->setdefaultsfunc = setdefaultsfunc_button;
    classe_button->geomnotifyfunc = geomnotifyfunc_button;
    classe_button->next = NULL;
    return classe_button;
}

// cette fonction enregistre une nouvelle class dans notre liste chaines de classe
void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass){
    ei_widgetclass_t* tete = liste_des_classe;
    if (tete == NULL){
        liste_des_classe = widgetclass;
    }
    else {
        while (tete->next != NULL) {
            tete = tete->next;
        }
        tete->next = widgetclass;
    }
}

// fonction permettant de retourner un pointeur vers une classe donnée
ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name){
    ei_widgetclass_t * tete = liste_des_classe;
    while(tete != NULL) {
        // ici, on compare le nom de la classe en tête de liste avec le nom de frame rechercher
        if (strcmp(name, tete->name) == 0) {
            return tete;
        }
        tete = tete->next;
    }
    return NULL;
}