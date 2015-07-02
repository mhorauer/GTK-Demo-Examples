/**
 * Classic GTK+ Menu Demo using a GtkApplication class
 *
 * M. Horauer
 */
#ifndef _bmd_classicmenu_
#define _bmd_classicmenu_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

typedef struct {
	GtkApplication *app;
	GtkWidget *window;
} bmd_widgets;

#endif
