//
// Created by traorefv on 30/04/24.
//

#include "ei_widget_configure.h"
#include "ei_implementation.h"
#include <stdbool.h>


bool verifie_si_null(void* chose_a_tester){
    return (chose_a_tester == NULL);
}
void			ei_frame_configure		(ei_widget_t		widget,
                                           ei_size_t*		requested_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_relief_t*		relief,
                                           ei_string_t*		text,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color,
                                           ei_anchor_t*		text_anchor,
                                           ei_surface_t*		img,
                                           ei_rect_ptr_t*		img_rect,
                                           ei_anchor_t*		img_anchor){



    widget = (ei_widget_t) widget;
    widget->requested_size = *requested_size;

    // on fait un transcriptage pour avoir accés aux autres champs
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;

    if ( !verifie_si_null(color) ){
        frame->color = *color;
    }

    if ( !verifie_si_null(border_width) ) {
        frame->border_width = *border_width;
    }
    if ( !verifie_si_null(relief) ) {
        frame->relief = *relief;
    }
    if ( !verifie_si_null(text) ) {
        frame->text = *text;
    }
    if ( !verifie_si_null(text_font) ) {
        frame->text_font = *text_font;
    }
    if ( !verifie_si_null(text_color) ) {
        frame->text_color = *text_color;
    }
    if ( !verifie_si_null(text_anchor) ) {
        frame->text_anchor = *text_anchor;
    }
    if ( !verifie_si_null(img) ) {
        frame->img = *img;
    }
    if ( !verifie_si_null(img_rect) ) {
        frame->img_rect = *img_rect;
    }
    if ( !verifie_si_null(img_anchor) ) {
        frame->img_anchor = *img_anchor;
    }


}

