#ifndef _bmd_editable_cells_callbacks_
#define _bmd_editable_cells_callbacks_

#include "bmd_editable_cells.h"

void bmd_add_items (void);
void bmd_add_item (GtkWidget *button, gpointer data);
void bmd_remove_item (GtkWidget *widget, gpointer data);
void bmd_cell_edited (GtkCellRendererText *cell,
		  const gchar         *path_string,
		  const gchar         *new_text,
		  gpointer             data);
void bmd_add_columns (GtkWidget  *widget, gpointer data);
GtkTreeModel *bmd_create_items_model (void);

#endif