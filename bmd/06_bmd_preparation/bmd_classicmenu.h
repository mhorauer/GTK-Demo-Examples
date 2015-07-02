/**
 * Classic GTK+ Menu Demo using a GtkApplication class
 *
 * M. Horauer
 */
#ifndef _classicmenu_
#define _classicmenu_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

typedef struct {
	GtkApplication *app;
	GtkWidget *window;
} bmd_widgets;

void
bmd_construct_menu (GtkApplication *app, GtkWidget *box, gpointer data);

#endif