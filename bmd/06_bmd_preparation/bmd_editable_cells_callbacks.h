#ifndef _editable_cells_callbacks_
#define _editable_cells_callbacks_

#include "bmd_editable_cells.h"

void bmd_add_items (void);
GtkTreeModel *bmd_create_numbers_model (void);
void bmd_add_item (GtkWidget *button, gpointer data);
void bmd_remove_item (GtkWidget *widget, gpointer data);
void bmd_cell_edited (GtkCellRendererText *cell,
		      const gchar         *path_string,
		      const gchar         *new_text,
		      gpointer             data);
void bmd_add_columns (GtkTreeView  *treeview,
		      GtkTreeModel *items_model,
		      GtkTreeModel *numbers_model);
GtkTreeModel* bmd_create_items_model (void);

#endif
