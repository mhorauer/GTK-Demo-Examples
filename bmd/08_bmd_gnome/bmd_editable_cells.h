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
#include "bmd_gnomemenu.h"

extern GArray *articles;

typedef struct {
	gint   number;
	gchar *title;
	gchar *author;
	gint   pages;
	gchar *imgfile;
}
Item;

enum {
	COLUMN_ITEM_NUMBER,
	COLUMN_ITEM_TITLE,
	COLUMN_ITEM_AUTHOR,
	COLUMN_ITEM_PAGES,
	COLUMN_ITEM_COVER,
	NUM_ITEM_COLUMNS
};

enum {
	COLUMN_NUMBER_TEXT,
	NUM_NUMBER_COLUMNS
};

void
bmd_construct_editablecells (GtkApplication *app, gpointer data);

#endif
