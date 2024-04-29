/**
 *  @file	ei_geometrymanager.h
 *  @brief	Manages the positioning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_GEOMETRYMANAGER_H
#define EI_GEOMETRYMANAGER_H

#include "ei_types.h"
#include "ei_widget.h"


/**
 * \brief	An opaque type that represents the geometry management parameters of a widget.
 */
struct ei_impl_geom_param_t;
typedef struct ei_impl_geom_param_t*	ei_geom_param_t;

/**
 * \brief	Returns the size of \ref ei_impl_geom_param_t.
 */
size_t		ei_geom_param_size();

/**
 * \brief	A name of a geometry manager.
 */
typedef char	ei_geometrymanager_name_t[20];

/**
 * \brief	A function that runs the geometry computation for this widget.
 * 		Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
typedef void	(*ei_geometrymanager_runfunc_t)		(ei_widget_t widget);

/**
 * \brief	A function called when a widget cease to be managed by its geometry manager.
 *		Most of the work is done in \ref ei_geometrymanager_unmap. This function hook is
 *		only provided to trigger recomputation when the disappearance of a widget has an
 *		effect on the geometry of other widgets.
 *
 * @param	widget		The widget instance that must be forgotten by the geometry manager.
 */
typedef void	(*ei_geometrymanager_releasefunc_t)	(ei_widget_t widget);

/**
 * \brief	The structure that stores information about a geometry manager.
 */
typedef struct ei_geometrymanager_t {
	ei_geometrymanager_name_t		name;
	ei_geometrymanager_runfunc_t		runfunc;
	ei_geometrymanager_releasefunc_t	releasefunc;
	struct ei_geometrymanager_t*		next;
} ei_geometrymanager_t;



/**
 * \brief	Must be called by runfuncs (see \ref ei_geometrymanager_runfunc_t).
 *
 * 		Checks if the geometry computation resulted in a change of geometry of
 * 		the widget (size and/or position). If there was a change:
 * 			* schedule a redraw of the screen on the old and new screen location
 * 			* notify the widget that it's geometry has changed
 * 			* recompute the geometry of the children
 *
 * @param	widget		The which geometry computation has been done.
 * @param	computed_screen_location The new screen location computed by the run of the
 * 				geometry manager. May be the same as the screen location of
 * 				the widget before the run.
 */
void			ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location);



/**
 * \brief	Registers a geometry manager to the program so that it can be called to manager
 *		widgets. This must be done only once in the application.
 *
 * @param	geometrymanager		The structure describing the geometry manager.
 */
void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager);



/**
 * \brief	Returns a geometry manager structure from its name.
 *
 * @param	name		The name of the geometry manager.
 *
 * @return			The structure describing the geometry manager.
 */
ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name);



/**
 * \brief	Tell the geometry manager in charge of a widget to forget it. This removes the
 *		widget from the screen. If the widget is not currently managed, this function
 *		returns silently.
 *		Side effects:
 *		<ul>
 *			<li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *				charge of this widget is called, </li>
 *			<li> the geom_param field of the widget is freed, </li>
 *			<li> the current screen_location of the widget is invalidated (which will
 *				trigger a redraw), </li>
 *			<li> the screen_location of the widget is reset to 0. </li>
 *		</ul>
 *
 * @param	widget		The widget to unmap from the screen.
 */
void			ei_geometrymanager_unmap	(ei_widget_t widget);



/**
 * @brief	Get the geometry manager for this widget.
 *
 * @param	widget		The widget.
 *
 * @return			A pointer to the geometry manager, or NULL if the widget is not currently displayed.
 */
ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget);

/**
 * @brief	Sets the geometry manager for this widget.
 *
 * @param	widget		The widget.
 * @param	manager		The geometry manager managing this widget.
 */
void			ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager);

/**
 * @brief	Get the geometry management parameters for this widget.
 *
 * @param	widget		The widget.
 *
 * @return			A pointer to the geometry management parameters, or NULL if the widget is not currently displayed.
 */
ei_geom_param_t		ei_widget_get_geom_params	(ei_widget_t widget);

/**
 * @brief	Sets the geometry management parameters for this widget.
 *
 * @param	widget		The widget.
 * @param	geom_params	The geometry management parameters.
 */
void			ei_widget_set_geom_params	(ei_widget_t widget, ei_geom_param_t geom_param);






#endif
