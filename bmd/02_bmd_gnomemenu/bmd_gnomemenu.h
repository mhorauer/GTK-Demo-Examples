/**
 * GNOME/GTK+ Menu Demo Application using a GtkApplication class
 *
 * M. Horauer
 */
#ifndef _bmd_gnomemenu_
#define _bmd_gnomemenu_


#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

typedef struct {
	GtkApplication *app;
	GtkWidget *window;
} bmd_widgets;

#endif