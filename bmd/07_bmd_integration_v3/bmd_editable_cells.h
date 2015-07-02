/*
 * Listview for Book Management Demo
 *
 * M. Horauer
 */
#ifndef _bmd_editable_cells_
#define _bmd_editable_cells_

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "bmd_classicmenu.h"

extern GArray *articles;

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
bmd_construct_editablecells (GtkApplication *app, gpointer data);

#endif