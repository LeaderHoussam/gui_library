//
// Created by ensimag on 5/18/24.
//

#include "ei_entry.h"

#include <ei_application.h>

#include "ei_implementation.h"

// configuratin d'un entry
void			ei_entry_configure		(ei_widget_t		widget,
                                           int*			requested_char_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color) {
    ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;

    if (text_font !=NULL ) {
        entry->text_font = *text_font;
    }

    if (requested_char_size != NULL) {
        int w, h;
        hw_text_compute_size("m", entry->text_font, &w,&h);
        entry->requested_char_size = *requested_char_size;
        widget->requested_size = (ei_size_t){*requested_char_size*w , h};
    }
    if(color != NULL) {
        entry->color = *color;
    }
    if (border_width != NULL) {
        entry->border_width = *border_width;
    }

    if ( text_color != NULL ) {
        entry->text_color = *text_color;
    }
    ei_app_invalidate_rect(&widget->screen_location);
}


void			ei_entry_set_text		(ei_widget_t		widget,
                             ei_const_string_t 	text) {

}

ei_const_string_t 	ei_entry_get_text		(ei_widget_t		widget) {
    return "";
}

void			ei_entry_give_focus		(ei_widget_t		widget) {

}

