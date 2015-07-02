#ifndef _editable_cells_
#define _editable_cells_

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#define STANDALONE 1

extern GArray *articles;


typedef struct {
	GtkApplication *app;
	GtkWidget *window;
} bmd_widgets;

typedef struct {
	gint   number;
	gchar *title;
	gchar *author;
	gint   pages;
}
Item;

enum {
	COLUMN_ITEM_NUMBER,
	COLUMN_ITEM_TITLE,
	COLUMN_ITEM_AUTHOR,
	COLUMN_ITEM_PAGES,
	NUM_ITEM_COLUMNS
};

enum {
	COLUMN_NUMBER_TEXT,
	NUM_NUMBER_COLUMNS
};

void
construct_editablecells (GtkApplication *app, GtkWidget *box, gpointer data);

#endif