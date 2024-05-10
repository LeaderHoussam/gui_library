//
// Created by ah aharbilh on 07 May 2024.
//
#include "ei_event.h"
#include "ei_types.h"

void		ei_bind			(ei_eventtype_t		eventtype,
                                ei_widget_t		widget,
                                ei_tag_t		tag,
                                ei_callback_t		callback,
                                void*			user_param){
    if (callback == NULL){
        return; //si le callback n'est pas valide.
    }
    if (tag!=NULL){

    } else {

    }
}

void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param);



