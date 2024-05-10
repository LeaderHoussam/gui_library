/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 * 
 */

#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"

/* par Nelson*/
typedef struct {
    int32_t				taille;
    ei_point_t*          points;
} ei_arc_t;

typedef struct {
    int32_t				taille_up;
    int32_t				taille_down;
    ei_point_t*          points_up;
    ei_point_t*          points_down;
} ei_arc_bg_t;

ei_arc_t* arc(int32_t rayon, ei_point_t centre, double angle_debut, double angle_fin);

ei_arc_t* rounded_frame(int32_t rayon, ei_rect_t rectangle);
ei_arc_bg_t* rounded_frame_bg(int32_t rayon, ei_rect_t rectangle, int32_t h);


/* fin par Nelson*/

/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_impl_widget_t {
    ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
    uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
    ei_color_t*		pick_color;	///< pick_id encoded as a color.
    void*			user_data;	///< Pointer provided by the programmer for private use. May be NULL.
    ei_widget_destructor_t	destructor;	///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

    /* Widget Hierachy Management */
    ei_widget_t		parent;		///< Pointer to the parent of this widget.
    ei_widget_t		children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
    ei_widget_t		children_tail;	///< Pointer to the last child of this widget.
    ei_widget_t		next_sibling;	///< Pointer to the next child of this widget's parent widget.

    /* Geometry Management */
    ei_geom_param_t		geom_params;	///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
    ei_size_t		requested_size;	///< See \ref ei_widget_get_requested_size.
    ei_rect_t		screen_location;///< See \ref ei_widget_get_screen_location.
    ei_rect_t*		content_rect;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.
} ei_impl_widget_t;



/**
 * @brief	Draws the children of a widget.
 * 		The children are draw withing the limits of the clipper and
 * 		the widget's content_rect.
 *
 * @param	widget		The widget which children are drawn.
 * @param	surface		A locked surface where to draw the widget's children.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                             ei_surface_t		surface,
                                             ei_surface_t		pick_surface,
                                             ei_rect_t*		clipper);



/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */
uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);


/**
 * \brief	Fields common to all geometry managers. Every geometry manager specializes this by adding its own fields.
 */
typedef struct ei_impl_geom_param_t {
    ei_geometrymanager_t*		manager;	///< The geometry managers that manages this widget.
} ei_impl_geom_param_t;

typedef struct ei_impl_placeur_t {
    ei_impl_geom_param_t geo_parameters;
    ei_anchor_t anchor;
    int x;
    int y;
    int width;
    int height;
    int rel_x;
    int rel_y;
    int rel_width;
    int rel_height;
}ei_impl_placeur_t;

typedef struct ei_impl_frame_t{
    ei_impl_widget_t widget;
    ei_color_t   color;
    int  	border_width;
    ei_relief_t  	relief;
    ei_string_t  	text;
    ei_font_t  	text_font;
    ei_color_t 	text_color;
    ei_anchor_t  	text_anchor;
    ei_surface_t  	img;
    ei_rect_ptr_t  	img_rect;
    ei_anchor_t 	img_anchor;

    // il faut ajouter ici l'attribut image,
    // mais je ne sais pas encore comment le faire
    // j'ai trouver en regardant dans ei_frame_configure

}ei_impl_frame_t;

typedef struct ei_impl_button_t{
    ei_impl_widget_t widget;
    ei_color_t   color;
    int  	border_width;
    int     corner_radius;
    ei_relief_t  	relief;
    ei_string_t  	text;
    ei_font_t  	text_font;
    ei_color_t 	text_color;
    ei_anchor_t  	text_anchor;
    ei_surface_t  	img;
    ei_rect_ptr_t  	img_rect;
    ei_anchor_t 	img_anchor;
    ei_callback_t   callback;
    ei_user_param_t	user_param;


}ei_impl_button_t;


// on va ajouter dans ce fichier, l'instanciation  de nos classes
ei_widgetclass_t* init_button_classe(void );
ei_widgetclass_t* init_frame_classe(void );
ei_geometrymanager_t*  init_placeur(void);
extern ei_widgetclass_t* liste_des_classe;
extern ei_surface_t root_window;
extern ei_widget_t root_widget;
extern ei_geometrymanager_t* liste_des_geometrie;
extern ei_linked_rect_t* surfaces_mises_a_jour;

// event gestion

/*
 * une structure ei_event_binding qui stocke les informations de liaison,
 * telles que le type d'événement, le widget ou l'étiquette, le rappel associé
 * et les paramètres utilisateur.
 */
struct ei_event_binding {
    ei_eventtype_t event_type;
    ei_widget_t* widget;
    ei_tag_t tag;
    ei_callback_t callback;
    void* user_param;
    struct ei_event_binding* next;
};

static struct ei_event_binding* EVENT_BINDINGS = NULL;


//Il faut corrigé callback
/*
ei_callback_t ei_event_get_callback(ei_eventtype_t event_type,
                                    ei_widget_t* widget,
                                    ei_user_param_t user_param) {
    struct ei_event_binding* binding = EVENT_BINDINGS;
    while (binding) {
        if (binding->event_type == event_type &&
            (binding->widget == widget || (binding->tag && strcmp(binding->tag,) == 0))) {
            return binding->callback;
        }
        binding = binding->next;
    }
    return NULL;
}
*/
/*
ei_callback_t ei_event_get_callback(ei_eventtype_t event_type,
                                    ei_widget_t widget,
                                    ei_user_param_t user_param) {
    // Obtenir le gestionnaire d'événements pour le type d'événement donné
    ei_callback_t callback = NULL;
    switch (event_type) {
        case ei_ev_keydown:
            callback = widget->wclass-> //handle_func.keydown;
            break;
        case ei_ev_keyup:
            callback = widget->wclass->handle_func.keyup;
            break;
        case ei_ev_mouse_buttondown:
            callback = widget->wclass->handle_func.mouse_down;
            break;
        case ei_ev_mouse_buttonup:
            callback = widget->wclass->handle_func.mouse_up;
            break;
        case ei_ev_mouse_move:
            callback = widget->wclass->handle_func.mouse_move;
            break;
        case ei_ev_mouse_wheel:
            callback = widget->wclass->handle_func.mouse_wheel;
            break;
        default:
            // Si aucun gestionnaire d'événements n'est défini pour ce type d'événement, utiliser la fonction de gestion par défaut
            callback = ei_event_get_default_handle_func();
            break;
    }
    return callback;
}*/

#endif
