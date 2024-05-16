//
// Created by ensimag on 5/14/24.
//

#include "ei_widget_attributes.h"
#include "ei_implementation.h"

void* ei_widget_get_user_data(ei_widget_t widget) {
    return widget->user_data;
}