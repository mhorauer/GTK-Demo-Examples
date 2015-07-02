/* Tree View/Editable Cells
 *
 * This is a modified example of the "editable-cells" demo out of the
 * gtk3-demo program.
 *
 * The modifications done are:
 *
 * (0) Move the type declarations to a separate header file.
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
 *                     G_CALLBACK (cell_edited), items_model);
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
 *     //                    G_CALLBACK (editing_started), NULL);
 *
 * Instead of (6) we could copy the block from the TITLES block and rename all
 * title entries with number entries. In this case, however, you need to add
 *   g_signal_connect (renderer, "edited",
 *                     G_CALLBACK (cell_edited), items_model);
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
 */

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "bmd_editable_cells.h"

static GtkWidget *window = NULL;
static GArray *articles = NULL;

static void
add_items (void)
{
	Item foo;

	g_return_if_fail (articles != NULL);

	foo.number = 1;
	foo.title = g_strdup ("Safari bei den Berggorillas");
	foo.author = g_strdup ("Erhard Dietl");
	foo.pages = 160;
	g_array_append_vals (articles, &foo, 1);

	foo.number = 2;
	foo.title = g_strdup ("Die krötigen Olchi Lieder");
	foo.author = g_strdup ("Olchi Opa");
	foo.pages = 42;
	g_array_append_vals (articles, &foo, 1);

	foo.number = 3;
	foo.title = g_strdup ("Die Gully Detektive von London");
	foo.author = g_strdup ("Olchi Papa");
	foo.pages = 260;
	g_array_append_vals (articles, &foo, 1);

	foo.number = 4;
	foo.title = g_strdup ("Im Bann des Magiers");
	foo.author = g_strdup ("Olchi Oma");
	foo.pages = 24;
	g_array_append_vals (articles, &foo, 1);
}

static GtkTreeModel *
create_items_model (void)
{
	gint i = 0;
	GtkListStore *model;
	GtkTreeIter iter;

	/* create array */
	articles = g_array_sized_new (FALSE, FALSE, sizeof (Item), 1);

	add_items ();

	/* create list store */
	model = gtk_list_store_new (NUM_ITEM_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
				    G_TYPE_INT, G_TYPE_BOOLEAN);

	/* add items */
	for (i = 0; i < articles->len; i++) {
		gtk_list_store_append (model, &iter);

		gtk_list_store_set (model, &iter,
				    COLUMN_ITEM_NUMBER,
				    g_array_index (articles, Item, i).number,
				    COLUMN_ITEM_TITLE,
				    g_array_index (articles, Item, i).title,
				    COLUMN_ITEM_AUTHOR,
				    g_array_index (articles, Item, i).author,
				    COLUMN_ITEM_PAGES,
				    g_array_index (articles, Item, i).pages,
				    -1);
	}

	return GTK_TREE_MODEL (model);
}

static GtkTreeModel *
create_numbers_model (void)
{
#define N_NUMBERS 10
	gint i = 0;
	GtkListStore *model;
	GtkTreeIter iter;

	/* create list store */
	model = gtk_list_store_new (NUM_NUMBER_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

	/* add numbers */
	for (i = 0; i < N_NUMBERS; i++) {
		char str[2];

		str[0] = '0' + i;
		str[1] = '\0';

		gtk_list_store_append (model, &iter);

		gtk_list_store_set (model, &iter,
				    COLUMN_NUMBER_TEXT, str,
				    -1);
	}

	return GTK_TREE_MODEL (model);

#undef N_NUMBERS
}

static void
add_item (GtkWidget *button, gpointer data)
{
	Item foo;
	GtkTreeIter current, iter;
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	GtkTreeView *treeview = (GtkTreeView *) data;

	g_return_if_fail (articles != NULL);

	foo.number = 0;
	foo.title = g_strdup ("Title here");
	foo.author = g_strdup ("Author here");
	foo.pages = 42;
	g_array_append_vals (articles, &foo, 1);

	/* Insert a new row below the current one */
	gtk_tree_view_get_cursor (treeview, &path, NULL);
	model = gtk_tree_view_get_model (treeview);
	if (path) {
		gtk_tree_model_get_iter (model, &current, path);
		gtk_tree_path_free (path);
		gtk_list_store_insert_after (GTK_LIST_STORE (model), &iter, &current);
	} else {
		gtk_list_store_insert (GTK_LIST_STORE (model), &iter, -1);
	}

	/* Set the data for the new row */
	gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			    COLUMN_ITEM_NUMBER, foo.number,
			    COLUMN_ITEM_TITLE, foo.title,
			    COLUMN_ITEM_AUTHOR, foo.author,
			    COLUMN_ITEM_PAGES, foo.pages,
			    -1);

	/* Move focus to the new row */
	path = gtk_tree_model_get_path (model, &iter);
	column = gtk_tree_view_get_column (treeview, 0);
	gtk_tree_view_set_cursor (treeview, path, column, FALSE);

	gtk_tree_path_free (path);
}

static void
remove_item (GtkWidget *widget, gpointer data)
{
	GtkTreeIter iter;
	GtkTreeView *treeview = (GtkTreeView *) data;
	GtkTreeModel *model = gtk_tree_view_get_model (treeview);
	GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

	if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
		gint i;
		GtkTreePath *path;

		path = gtk_tree_model_get_path (model, &iter);
		i = gtk_tree_path_get_indices (path) [0];
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);

		g_array_remove_index (articles, i);

		gtk_tree_path_free (path);
	}
}

/*
static gboolean
separator_row (GtkTreeModel *model,
               GtkTreeIter  *iter,
               gpointer      data)
{
  GtkTreePath *path;
  gint idx;

  path = gtk_tree_model_get_path (model, iter);
  idx = gtk_tree_path_get_indices (path)[0];

  gtk_tree_path_free (path);

  return idx == 5;
}


static void
editing_started (GtkCellRenderer *cell,
                 GtkCellEditable *editable,
                 const gchar     *path,
                 gpointer         data)
{
  gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX (editable),
                                        separator_row, NULL, NULL);
}
*/

static void
cell_edited (GtkCellRendererText *cell,
	     const gchar         *path_string,
	     const gchar         *new_text,
	     gpointer             data)
{
	GtkTreeModel *model = (GtkTreeModel *) data;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
	GtkTreeIter iter;

	gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

	gtk_tree_model_get_iter (model, &iter, path);

	switch (column) {
	case COLUMN_ITEM_NUMBER: {
		gint i;

		i = gtk_tree_path_get_indices (path) [0];
		g_array_index (articles, Item, i).number = atoi (new_text);

		gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
				    g_array_index (articles, Item, i).number, -1);
	}
	break;

	case COLUMN_ITEM_TITLE: {
		gint i;
		gchar *old_text;

		gtk_tree_model_get (model, &iter, column, &old_text, -1);
		g_free (old_text);

		i = gtk_tree_path_get_indices (path) [0];
		g_free (g_array_index (articles, Item, i).title);
		g_array_index (articles, Item, i).title = g_strdup (new_text);

		gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
				    g_array_index (articles, Item, i).title, -1);
	}
	break;

	case COLUMN_ITEM_AUTHOR: {
		gint i;
		gchar *old_text;

		gtk_tree_model_get (model, &iter, column, &old_text, -1);
		g_free (old_text);

		i = gtk_tree_path_get_indices (path) [0];
		g_free (g_array_index (articles, Item, i).author);
		g_array_index (articles, Item, i).author = g_strdup (new_text);

		gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
				    g_array_index (articles, Item, i).author, -1);
	}
	break;

	case COLUMN_ITEM_PAGES: {
		gint i;

		i = gtk_tree_path_get_indices (path) [0];
		g_array_index (articles, Item, i).pages = atoi (new_text);

		gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
				    g_array_index (articles, Item, i).pages, -1);
	}
	break;

	}

	gtk_tree_path_free (path);
}

static void
add_columns (GtkTreeView  *treeview,
	     GtkTreeModel *items_model,
	     GtkTreeModel *numbers_model)
{
	GtkCellRenderer *renderer;

	/* number column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_NUMBER));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Number", renderer,
						     "text", COLUMN_ITEM_NUMBER,
						     NULL);

	/* title column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_TITLE));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Title", renderer,
						     "text", COLUMN_ITEM_TITLE,
						     NULL);

	/* author column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_AUTHOR));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Author", renderer,
						     "text", COLUMN_ITEM_AUTHOR,
						     NULL);

	/* pages column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_PAGES));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Pages", renderer,
						     "text", COLUMN_ITEM_PAGES,
						     NULL);
}

GtkWidget *
do_editable_cells (GtkWidget *do_widget)
{
	if (!window) {
		GtkWidget *vbox;
		GtkWidget *hbox;
		GtkWidget *sw;
		GtkWidget *treeview;
		GtkWidget *button;
		GtkTreeModel *items_model;
		GtkTreeModel *numbers_model;

		/* create window, etc */
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
//      gtk_window_set_screen (GTK_WINDOW (window),
//                             gtk_widget_get_screen (do_widget));
		gtk_window_set_title (GTK_WINDOW (window), "Booklist");
		gtk_container_set_border_width (GTK_CONTAINER (window), 5);
		g_signal_connect (window, "destroy",
				  G_CALLBACK (gtk_widget_destroyed), &window);

		vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
		gtk_container_add (GTK_CONTAINER (window), vbox);

//      gtk_box_pack_start (GTK_BOX (vbox),
//                          gtk_label_new ("Shopping list (you can edit the cells!)"),
//                          FALSE, FALSE, 0);

		sw = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
						     GTK_SHADOW_ETCHED_IN);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
						GTK_POLICY_AUTOMATIC,
						GTK_POLICY_AUTOMATIC);
		gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);

		/* create models */
		items_model = create_items_model ();
		numbers_model = create_numbers_model ();

		/* create tree view */
		treeview = gtk_tree_view_new_with_model (items_model);
		gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview)),
					     GTK_SELECTION_SINGLE);

		add_columns (GTK_TREE_VIEW (treeview), items_model, numbers_model);

		g_object_unref (numbers_model);
		g_object_unref (items_model);

		gtk_container_add (GTK_CONTAINER (sw), treeview);

		/* some buttons */
		hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
		gtk_box_set_homogeneous (GTK_BOX (hbox), TRUE);
		gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

		button = gtk_button_new_with_label ("Add item");
		g_signal_connect (button, "clicked",
				  G_CALLBACK (add_item), treeview);
		gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

		button = gtk_button_new_with_label ("Remove item");
		g_signal_connect (button, "clicked",
				  G_CALLBACK (remove_item), treeview);
		gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

		gtk_window_set_default_size (GTK_WINDOW (window), 320, 200);
	}

	if (!gtk_widget_get_visible (window))
		gtk_widget_show_all (window);
	else {
		gtk_widget_destroy (window);
		window = NULL;
	}

	return window;
}

/* code added */
int
main (int argc, char **argv)
{
	GtkWidget *window;

	gtk_init (&argc, &argv);
	window = do_editable_cells (NULL);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_main();
	return 0;
}
/*! EOF */


