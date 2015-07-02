/*
 * Listview for Book Management Demo
 *
 * M. Horauer
 */

#include "bmd_editable_cells.h"
#include "bmd_editable_cells_callbacks.h"

GArray *articles = NULL;


/************************************************************* Program Layout */
void
bmd_construct_editablecells (GtkApplication *app, gpointer data)
{
	GtkWidget *sw;
	bmd_widgets *a = (bmd_widgets *) data;

	sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
					     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request (sw, XSIZE, YSIZE - 310);
	gtk_paned_pack1 (GTK_PANED (a->paned), sw, TRUE, FALSE);

	/* create models */
	a->items_model = bmd_create_items_model();
	/* create tree view */
	a->treeview = gtk_tree_view_new_with_model (a->items_model);
	gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (a->treeview)),
				     GTK_SELECTION_SINGLE);
	bmd_add_columns (NULL, (gpointer) a);
	g_object_unref (a->items_model);
	gtk_container_add (GTK_CONTAINER (sw), a->treeview);
}
/*! EOF */


