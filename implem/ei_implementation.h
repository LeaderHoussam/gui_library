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
#include "ei_placer.h"

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
ei_arc_t* rounded_top_level(int32_t rayon, ei_rect_t rectangle);
ei_arc_bg_t* triangle_frame_bg(ei_rect_t rectangle);
ei_point_t*  place_text ( ei_widget_t widget, ei_const_string_t	text, const ei_font_t	font, ei_anchor_t text_anchor);

ei_rect_ptr_t place_img (  ei_widget_t widget, ei_surface_t img, ei_rect_ptr_t img_rect, ei_anchor_t img_anchor);

void free_place_text( ei_point_t* point );

void free_place_img( ei_rect_ptr_t rect );

void compare_rect(ei_widget_t widget, ei_rect_ptr_t source, ei_anchor_t img_anchor);



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
    float rel_x;
    float rel_y;
    float rel_width;
    float rel_height;
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

typedef struct ei_impl_toplevel_t{
    ei_impl_widget_t widget;
    ei_color_t   color;
    int  	border_width;
    ei_string_t  	title;
    bool 	closable;
    ei_axis_set_t resizable;
    ei_size_ptr_t  	min_size;
}ei_impl_toplevel_t;


// on va ajouter dans ce fichier, l'instanciation  de nos classes
ei_widgetclass_t* init_toplevel_classe(void);
ei_widgetclass_t* init_button_classe(void );
ei_widgetclass_t* init_frame_classe(void );
ei_geometrymanager_t*  init_placeur(void);
extern ei_widgetclass_t* liste_des_classe;
extern ei_surface_t root_window;
extern ei_surface_t offscreen;
extern ei_widget_t root_widget;

//extern ei_geometrymanager_t* liste_des_geometrie;
extern ei_geometrymanager_t* liste_de_gestionnaires_geom;
extern ei_linked_rect_t* surfaces_mise_a_jour;
extern ei_rect_t* clipper_final;


extern uint32_t compteur_pick_id;

//fonction pour transformer un pick_id en couleur
ei_color_t* map_pick_id_to_color(ei_surface_t surface, uint32_t pick_id);

/*
// pour capter les données de l'evenement:
extern ei_event_t* evenement;

// ICI JE VAIS DEFINIR DES TYPES GENERAUX POUR CAPTER
// POUR LIER DES TRAITANTS À CHAQUE ÉVÈNEMENTS
// ON PENSERA BIEN À CE QUE NOS TRAITANTS INTERNES RENVOIE
// FALSE POUR POUVOIR TRAITER LES TRAITANTS EXTERNES
// QUE L'UTILISATEUR AJOUTERA

// concretement, chaque évènement aura une liste chainées de traitants
// et ei_bind va ajouter des traitants à la liste des traitants d'un évènement
// on va créer une structure xxx dont les elements sont les paramètres de bind ou unbind
// on va aussi créer une fonction execute structure qui verifie si le traitant doit etre executer et le fait:
// donc chaque evènements aura une liste chainées de xxx
// bind ajoute un éléments de type xxx à cette liste et unbid l'en suprimme
// on et on appelera execute sur les évènements de cette liste

// j'appelle la structure traitant_t
typedef struct traitant_t {
    //ei_eventtype_t evenement;
    ei_widget_t widget;
    ei_tag_t tag;
    ei_callback_t callback;
    void* user_param;
    struct traitant_t* next;
}traitant_t;

// on cree aussi une structure de transcriptage d'un évènement
typedef struct event_with_callback {
    ei_eventtype_t event;
    traitant_t* liste_des_traitants;
    struct event_with_callback* next;
}event_with_callback;

extern event_with_callback* liste_des_events_enregistres;
bool execute_traitant(ei_event_t* event, traitant_t traitant);
traitant_t* trouve_traitant(ei_eventtype_t eventtype);
ei_widget_t get_widget_actuel(ei_event_t* event);
ei_color_t* get_pick_screen_color(ei_point_t pos_souris);
ei_widget_t get_widget_from_pick_color(ei_color_t pick_color);
*/


/*
 * une structure ei_event_binding qui stocke les informations de liaison,
 * telles que le type d'événement, le widget ou l'étiquette, le rappel associé
 * et les paramètres utilisateur.
 */
typedef struct ei_event_binding {
    ei_eventtype_t event_type;
    ei_widget_t widget;
    ei_tag_t tag;
    ei_callback_t callback;
    void* user_param;
    struct ei_event_binding* next;
}ei_event_binding;
bool execute_traitant(ei_event_t* event,ei_event_binding traitant);
extern   ei_event_binding* EVENT_BINDINGS;

extern ei_surface_t offscreen;

extern bool top_toplevel;
extern bool btm_toplevel;
extern ei_point_t pt_init_toplevel;
static inline void ei_place_wh		(ei_widget_t widget, int w, int h){
    ei_place(widget, NULL, NULL, NULL, &w, &h, NULL, NULL, NULL, NULL);
}
typedef struct link_widget {
    ei_widget_t widget;
    struct link_widget* next;
}link_widget;
extern link_widget* liste_des_widgets;
bool bouton_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
bool bouton_handler_1(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
bool toplevel_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
bool toplevel_handler_1(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
bool toplevel_handler_2(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
bool toplevel_redimension(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

extern ei_point_t pos_mouse;

//static inline void ei_place_wh		(ei_widget_t widget, int w, int h)			{ ei_place(widget, NULL, NULL, NULL, &w, &h, NULL, NULL, NULL, NULL); }
void liberer_ei_linked_rect(ei_linked_rect_t** liste);
void liberer_ei_rect(ei_rect_t** clip);
ei_rect_t* trouve_rect_contenant(ei_rect_t rec1, ei_rect_t rec2);
ei_rect_t* trouve_inter_rect(ei_rect_t rect1, ei_rect_t rect2);

int min(int a, int b);
int max(int a, int b);

void delete_widget(ei_widget_t widget);
bool color_equal(ei_color_t *widget_color, ei_color_t pixel_color);
ei_widget_t pick_recursive(ei_widget_t widget, ei_color_t* color);
void move_widget_to_end(ei_widget_t widget);
void append_widget(ei_widget_t parent, ei_widget_t widget);

//strcmp
bool compare_widget_class_name_and_tag(ei_widgetclass_name_t name, char *tag);
#endif
