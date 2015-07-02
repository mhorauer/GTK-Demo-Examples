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
  GtkWidget *box;
  GtkWidget *paned;
  GtkWidget *image; 
  GtkWidget *statusbar;
  GtkWidget *treeview;
  GtkTreeModel *items_model;
  GtkTreeModel *numbers_model;
} bmd_widgets;

void
bmd_construct_menu(GtkApplication *app, gpointer data);

#endif