/**
 *  @file	ei_event.h
 *  @brief	Allows the binding and unbinding of callbacks to events.
 *
 *  \author 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */
#ifndef EI_EVENT_H
#define EI_EVENT_H

#include "ei_types.h"
#include "ei_widget.h"


/**
 * @brief	A string that can be attached to a widget. All widget have the tag of the name of
 *		their widget class, and the tag "all".
 */
typedef ei_string_t ei_tag_t;

/**
 * @brief	A tag and a pointer to create a linked list.
 */
typedef struct ei_linked_tag_t {
	ei_tag_t		tag;
	struct ei_linked_tag_t*	next;
} ei_linked_tag_t;

/**
 * @brief	The types of events.
 */
typedef enum {
	ei_ev_none		= 0,	///< No event, used to initialize the structure.
	ei_ev_app,			///< An application event, created by \ref hw_event_post_app.
	ei_ev_close,			///< The user requested to close the application's window.
	ei_ev_exposed,			///< The application's window has been exposed and should be redrawn.
	ei_ev_keydown,			///< A keyboard key has been pressed.
	ei_ev_keyup,			///< A keyboard key has been released.
	ei_ev_text_input,		///< Some text was input with the keyboard.

	/* event types at or above "ei_ev_mouse_buttondown" are mouse events that require picking. */
	ei_ev_mouse_buttondown,		///< A mouse button has been pressed.
	ei_ev_mouse_buttonup,		///< A mouse button has been released.
	ei_ev_mouse_move,		///< The mouse has moved.
	ei_ev_mouse_wheel,		///< The mouse wheel was rolled.

	ei_ev_last			///< Last event type, its value is the number of event types.
} ei_eventtype_t;

/**
 * @brief	The modifier keys (shift, alt, etc.)
 */
typedef enum {
	ei_mod_shift_left	= 0,	///< The "shift" key at the left of the space bar.
	ei_mod_alt_left,		///< The "alternate" key at the left of the space bar.
	ei_mod_meta_left,		///< The "meta" (command) key at the left of the space bar.
	ei_mod_ctrl_left,		///< The "control" key at the left of the space bar.
	ei_mod_shift_right,		///< The "shift" key at the right of the space bar.
	ei_mod_alt_right,		///< The "alternate" key at the right of the space bar.
	ei_mod_meta_right,		///< The "meta" (command) key at the right of the space bar.
	ei_mod_ctrl_right		///< The "control" key at the right of the space bar.

} ei_modifier_key_t;

/**
 * @brief	A bitfield indicating which of the modifier keys are currently pressed.
 * 		e.g., if the 4th bit is 1, it means that the "control" key on the left side of the keyboard
 * 		was pressed at the time of the event (\ref ei_mod_ctrl_left is the 4th identifier in the
 * 		enumerate \ref ei_modifier_key_t).
 */
typedef uint32_t			ei_modifier_mask_t;

/**
 * @brief	The various mouse buttons.
 */
typedef enum {
	ei_mouse_button_left,		///< The left mouse button.
	ei_mouse_button_middle,		///< The middle mouse button.
	ei_mouse_button_right		///< The right mouse button.
} ei_mouse_button_t;

/**
 * @brief	The event parameter for mouse-related event types.
 */
typedef struct {
	ei_point_t			where;		///< Where the mouse pointer was at the time of the event.
	ei_mouse_button_t 		button;		///< The mouse button that was pressed or released. Only valid for \ref ei_ev_mouse_buttondown or \ref ei_ev_mouse_buttonup event types.
	float 				wheel;		///< The amount of wheel rotation. Only valid for \ref ei_ev_mouse_wheel event types.
} ei_mouse_event_t;

/**
 * @brief	The event parameter for application defined event types.
 */
typedef struct {
	void*				user_param;
} ei_app_event_t;

/**
 * @brief	Describes an event.
 */
typedef struct ei_event_t {
	ei_eventtype_t			type;		///< The type of the event.
	ei_modifier_mask_t 		modifier_mask;	///< which of the modifier keys where pressed at the time of the event.
	union {
		char 			text;		///< Event parameter for \ref ei_ev_text_input events.
		SDL_Keycode		key_code;	///< Event parameters for \ref ei_ev_keydown and \ref ei_ev_keyup event.
		ei_mouse_event_t	mouse;		///< Event parameters for mouse-related	events (see \ref ei_mouse_event_t).
		ei_app_event_t		application;	///< Event parameters for application-related events (see \ref ei_app_event_t).
	} param;
} ei_event_t;

/**
 * @brief	Tests is a modifier key is noted as "pressed" in a modifier mask.
 *
 * @param	modifier_mask	The modifier mask (see \ref ei_modifier_mask_t).
 * @param	modifier	The modifier key (see \ref ei_modifier_key_t).
 *
 * @return			true if this modifier key was pressed, false otherwise.
 */
static inline bool		ei_mask_has_modifier(ei_modifier_mask_t	modifier_mask,
						     ei_modifier_key_t	modifier)
							{ return (bool)(modifier_mask & (1 << modifier)); }

 static inline bool		ei_event_has_shift(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right); }
 static inline bool		ei_event_has_alt(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_alt_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_alt_right); }
 static inline bool		ei_event_has_ctrl(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right); }
 static inline bool		ei_event_has_meta(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_meta_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_meta_right); }



/**
 * \brief	Binds a callback to an event type and a widget or a tag.
 *
 * @param	eventtype	The type of the event.
 * @param	widget		The callback is only called if the event is related to this widget.
 *				This parameter must be NULL if the "tag" parameter is not NULL.
 * @param	tag		The callback is only called if the event is related to a widget that
 *				has this tag. A tag can be a widget class name, or the tag "all".
 *				This parameter must be NULL is the "widget" parameter is not NULL.
 * @param	callback	The callback (i.e. the function to call).
 * @param	user_param	A user parameter that will be passed to the callback when it is called.
 */
void		ei_bind			(ei_eventtype_t		eventtype,
					 ei_widget_t		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param);

/**
 * \brief	Unbinds a callback from an event type and widget or tag.
 *
 * @param	eventtype, widget, tag, callback, user_param
 *				All parameters must have the same value as when \ref ei_bind was
 *				called to create the binding.
 */
void		ei_unbind		(ei_eventtype_t		eventtype,
					 ei_widget_t		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param);



#endif
