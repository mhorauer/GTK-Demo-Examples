#include "bmd_editable_cells.h"
#include "bmd_editable_cells_callbacks.h"

void
bmd_add_items (void)
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

GtkTreeModel *
bmd_create_items_model (void)
{
	gint i = 0;
	GtkListStore *model;
	GtkTreeIter iter;

	/* create array */
	articles = g_array_sized_new (FALSE, FALSE, sizeof (Item), 1);

	bmd_add_items ();

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

GtkTreeModel *
bmd_create_numbers_model (void)
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

void
bmd_add_item (GtkWidget *button, gpointer data)
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

void
bmd_remove_item (GtkWidget *widget, gpointer data)
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

void
bmd_cell_edited (GtkCellRendererText *cell,
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

void
bmd_add_columns (GtkTreeView  *treeview,
		 GtkTreeModel *items_model,
		 GtkTreeModel *numbers_model)
{
	GtkCellRenderer *renderer;
	// added for sorting
	GtkTreeViewColumn *column;


	/* number column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (bmd_cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_NUMBER));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Number", renderer,
						     "text", COLUMN_ITEM_NUMBER,
						     NULL);
	// added for sorting
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 0);
	gtk_tree_view_column_set_sort_column_id (column, 0);


	/* title column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (bmd_cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_TITLE));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Title", renderer,
						     "text", COLUMN_ITEM_TITLE,
						     NULL);
	// added for sorting
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 1);
	gtk_tree_view_column_set_sort_column_id (column, 1);

	/* author column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (bmd_cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_AUTHOR));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Author", renderer,
						     "text", COLUMN_ITEM_AUTHOR,
						     NULL);
	// added for sorting
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 2);
	gtk_tree_view_column_set_sort_column_id (column, 2);

	/* pages column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (bmd_cell_edited), items_model);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_PAGES));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Pages", renderer,
						     "text", COLUMN_ITEM_PAGES,
						     NULL);
	// added for sorting
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 3);
	gtk_tree_view_column_set_sort_column_id (column, 3);

}
/*! EOF */