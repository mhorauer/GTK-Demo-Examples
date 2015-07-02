/**
 * GNOME/GTK+ Menu Demo Application using a GtkApplication class
 *
 * M. Horauer
 */
#ifndef _bmd_gnomemenu_
#define _bmd_gnomemenu_


#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define XSIZE 600
#define YSIZE 600

typedef struct {
	GtkApplication *app;
	GtkWidget *window;
	GtkWidget *box;
	GtkWidget *paned;
	GtkWidget *image;
	GtkWidget *statusbar;
	GtkWidget *treeview;
	GtkListStore *model;
	GtkTreeModel *items_model;
	char *filename;
} bmd_widgets;

void
bmd_construct_menu (GtkApplication *app, gpointer data);

#endif
