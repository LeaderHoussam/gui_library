//
// Created by traorefv on 30/04/24.
//
#include "ei_placer.h"
#include "ei_implementation.h"

void		ei_place	(ei_widget_t		widget,
                         ei_anchor_t*		anchor,
                         int*			x,
                         int*			y,
                         int*			width,
                         int*			height,
                         float*			rel_x,
                         float*			rel_y,
                         float*			rel_width,
                         float*			rel_height){
    if (widget->geom_params != NULL) {
        ei_impl_placeur_t* un_placeur = (ei_impl_placeur_t*) widget->geom_params;
        if(anchor != NULL) {
            un_placeur->anchor = *anchor;
        }
        if(x != NULL) {
            un_placeur->x = *x;
        }
        if(y != NULL) {
            un_placeur->y = *y;
        }
        if(width != NULL) {
            un_placeur->width = *width;
        }
        if(height != NULL) {
            un_placeur->height = *height;
        }
        if(rel_x != NULL) {
            un_placeur->rel_x = *rel_x;
        }
        if(rel_y != NULL) {
            un_placeur->rel_y = *rel_y;
        }
        if(rel_width != NULL) {
            un_placeur->rel_width = *rel_width;
        }
        if(rel_height!= NULL) {
            un_placeur->rel_height = *rel_height;
        }
    }
    else {
        ei_impl_placeur_t* un_placeur =  calloc(1,sizeof(ei_impl_placeur_t));
        if(un_placeur == NULL) {
            perror("erreur allocation");
            exit(EXIT_FAILURE);
        }
        // default params
        un_placeur->anchor = (anchor == NULL) ? ei_anc_northwest : *anchor;
        un_placeur->x = (x == NULL) ? 0 : *x;
        un_placeur->y = (y == NULL) ? 0 : *y;
        un_placeur->width = (width == NULL) ? ((rel_width==NULL)?widget->requested_size.width : 0) : *width;
        un_placeur->height = (height == NULL) ? ((rel_height==NULL)?widget->requested_size.height : 0) : *height;
        un_placeur->rel_x = (rel_x == NULL) ? 0 : *rel_x;
        un_placeur->rel_y = (rel_y == NULL) ? 0 : *rel_y;
        un_placeur->rel_width = (rel_width == NULL) ? 0.0 : *rel_width;
        un_placeur->rel_height = (rel_height == NULL) ? 0.0 : *rel_height;

        widget->geom_params = (ei_impl_geom_param_t*) un_placeur;
        widget->geom_params->manager= ei_geometrymanager_from_name("placeur");
    }
    widget->geom_params->manager->runfunc(widget);
}
