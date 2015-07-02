/* Tree View/Editable Cells
 *
 * This is a modified example of the "editable-cells" demo out of the
 * gtk3-demo program.
 *
 * The modifications done are:
 *
 * (1) Replace/add the following strings and variables:
 *
 * Replace:
 * --------
 * Product -> Title
 * product -> title
 * COLUMN_ITEM_PRODUCT -> COLUMN_ITEM_TITLE
 *
 * Add (by copying the "product" snippets):
 * ----------------------------------------
 * Author
 * author
 * COLUMN_ITEM_AUTHOR
 *
 * Yummy -> Pages
 * yummy -> pages
 * COLUMN_ITEM_YUMMY -> COLUMN_ITEM_PAGES
 *
 * (2) In the two function calls to gtk_list_store_new() add G_TYPE_STRING in
 * between for the AUTHOR field.
 *
 * (3) In the callback function cell_edited() add a CASE statement for
 * COLUMN_ITEM_AUTHOR by copying the original one for COLUMN_ITEM_PRODUCT.
 * Add a CASE statement for COLUMN_ITEM_PAGES by copying the original one for
 * COLUMN_ITEM_NUMBER.
 *
 * (4) In the callback function add_columns() we change the following function
 * invocations:
 *
 * gtk_cell_renderer_progress_new -> gtk_cell_renderer_text_new
 * gtk_cell_renderer_combo_new -> gtk_cell_renderer_text_new
 *
 * (5) We need to add the following function call below the modified
 * gtk_cell_renderer_progress_new() ...
 *
 *   g_signal_connect (renderer, "edited",
 *                     G_CALLBACK (bmd_cell_edited), items_model);
 *
 * (6) Below the original function invocation of gtk_cell_renderer_combo_new()
 * do:
 *   - Add the following comments
 *       g_object_set (renderer,
 *         //          "model", numbers_model,
 *         //          "text-column", COLUMN_NUMBER_TEXT,
 *         //          "has-entry", FALSE,
 *                     "editable", TRUE,
 *                     NULL);
 *
 *   - Comment/remove
 *     //  g_signal_connect (renderer, "editing-started",
 *     //                    G_CALLBACK (bmd_editing_started), NULL);
 *
 * Instead of (6) we could copy the block from the TITLES block and rename all
 * title entries with number entries. In this case, however, you need to add
 *   g_signal_connect (renderer, "edited",
 *                     G_CALLBACK (bmd_cell_edited), items_model);
 *
 * (7) Since they are not needed anymore - comment/remove the entire functions
 * separator_row() and editing_started().
 *
 * (8) Finally, in the do_editable_cells() function we remove the label -
 * comment/remove:
 *
 * // gtk_box_pack_start (GTK_BOX (vbox),
 * //                 gtk_label_new ("Shopping list (you can edit the cells!)"),
 * //                 FALSE, FALSE, 0);
 *
 * (9) Added SORTING functionality in the add_columns() function - add multiple
 *     lines like:
 *
 *   // added for sorting
 *   GtkTreeViewColumn *column;
 *   // [....]
 *
 *   // added for sorting
 *   column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 1);
 *   gtk_tree_view_column_set_sort_column_id (column, 1);
 *
 *   NOTE: You'll need to replace 1 with the number of the column.
 */

#include "bmd_editable_cells.h"
#include "bmd_editable_cells_callbacks.h"

GArray *articles = NULL;


/************************************************************* Program Layout */
void
construct_editablecells (GtkApplication *app, GtkWidget *box, gpointer data)
{
	GtkWidget *hbox;
	GtkWidget *sw;
	GtkWidget *treeview;
	GtkWidget *button;
	GtkTreeModel *items_model;
	GtkTreeModel *numbers_model;

	sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
					     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (box), sw, TRUE, TRUE, 0);

	/* create models */
	items_model = bmd_create_items_model ();
	numbers_model = bmd_create_numbers_model ();

	/* create tree view */
	treeview = gtk_tree_view_new_with_model (items_model);
	gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview)),
				     GTK_SELECTION_SINGLE);

	bmd_add_columns (GTK_TREE_VIEW (treeview), items_model, numbers_model);

	g_object_unref (numbers_model);
	g_object_unref (items_model);

	gtk_container_add (GTK_CONTAINER (sw), treeview);

	/* some buttons */
	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
	gtk_box_set_homogeneous (GTK_BOX (hbox), TRUE);
	gtk_box_pack_start (GTK_BOX (box), hbox, FALSE, FALSE, 0);

	button = gtk_button_new_with_label ("Add item");
	g_signal_connect (button, "clicked",
			  G_CALLBACK (bmd_add_item), treeview);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

	button = gtk_button_new_with_label ("Remove item");
	g_signal_connect (button, "clicked",
			  G_CALLBACK (bmd_remove_item), treeview);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

}

#if STANDALONE
// app bmd_activate callback - creates the window
static void
bmd_activate (GtkApplication* app,
	      gpointer        user_data)
{
	GtkWidget *vbox;
	bmd_widgets *a = (bmd_widgets *) user_data;

	/* create window, etc */
	a->window = gtk_application_window_new (a->app);
	gtk_window_set_title (GTK_WINDOW (a->window), "Book Management");
	gtk_container_set_border_width (GTK_CONTAINER (a->window), 5);
	gtk_window_set_default_size (GTK_WINDOW (a->window), 800, 400);
	gtk_window_set_default_icon_from_file ("bmd_icon.png", NULL);

	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_add (GTK_CONTAINER (a->window), vbox);

	/* moved the creation of the program to a dedicated function */
	construct_editablecells (app, GTK_WIDGET (vbox), (gpointer) a);

	gtk_widget_show_all (a->window);
}

/* code added */
int
main (int argc, char **argv)
{
	int status;

	bmd_widgets *a = g_malloc (sizeof (bmd_widgets));

	// create a threaded application
	a->app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (a->app, "activate", G_CALLBACK (bmd_activate), (gpointer) a);
	// run the application -> emits an "bmd_activate" signal
	status = g_application_run (G_APPLICATION (a->app), argc, argv);
	g_object_unref (a->app);

	g_free (a);
	return status;
}
#endif
/*! EOF */


