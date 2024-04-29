//
// Created by François Bérard on 15/04/2024.
//

#ifndef EI_ENTRY_H
#define EI_ENTRY_H

#include "ei_types.h"

/**
 * @brief	Configures the attributes of widgets of the class "entry".
 *
 * @param	widget		The widget to configure.
 * @param	requested_char_size	The size requested for this widget in "chars". If provided, the requested size
 * 					of the widget must be just large enough to show this amount of 'm' characters
 * 					(the largest character). Defaults to 10.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 2. Minimum is 2.
 * 					When the widget does not have the focus, its border is shown with a
 * 					width of <border_width> - 1, but the content is enlarged so that
 * 					the widget has the same size as when in focus.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to \ref ei_font_default_color.
 */
void			ei_entry_configure		(ei_widget_t		widget,
							 int*			requested_char_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color);

/**
 * @brief	Sets the text displayed in the entry widget.
 *
 * @param	widget		The widget to set.
 * @param	text		The text to show in the widget.
 */
void			ei_entry_set_text		(ei_widget_t		widget,
							 ei_const_string_t 	text);

/**
 * @brief	Gets the text that is displayed in the entry widget.
 *
 * @param	widget		The widget to get the text from.
 *
 * @return			The text currently shown in the widget.
 */
ei_const_string_t 	ei_entry_get_text		(ei_widget_t		widget);

/**
 * @brief	Gives the keyboard input focus to the entry widget.
 *
 * @param	widget		The widget to receive the keyboard input focus.
 */
void			ei_entry_give_focus		(ei_widget_t		widget);

#endif //EI_ENTRY_H
