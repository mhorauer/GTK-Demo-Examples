/*
 * Callbacks for Listview of Book Management Demo
 *
 * M. Horauer
 */
#include "bmd_editable_cells.h"
#include "bmd_editable_cells_callbacks.h"
#include "bmd_image_statusbar.h"

void
bmd_add_items (GFile *filename, gpointer data)
{
	Item foo;
	gchar *contents;
	gsize length;
	gchar *line, *str;
	gchar *end_str, *end_token;
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	GtkTreeIter current, iter;

	bmd_widgets *a = (bmd_widgets *) data;
	GtkTreeView *treeview = (GtkTreeView *) a->treeview;

	g_return_if_fail (articles != NULL);
	if (filename != NULL) {
		// FIXME: Add some checks for the file - right now it assumes a well-formed
		//        format
		if (g_file_load_contents (filename, NULL, &contents, &length, NULL, NULL)) {
			line = strtok_r (contents, "\n", &end_str);
			while (line != NULL) {
				str = g_strdup (line);
				foo.number = atoi (strtok_r (str, ",", &end_token));
				foo.title = g_strdup (strtok_r (NULL, ",", &end_token));
				foo.author = g_strdup (strtok_r (NULL, ",", &end_token));
				foo.pages = atoi (strtok_r (NULL, ",", &end_token));
				foo.imgfile = g_strdup (strtok_r (NULL, ",", &end_token));
				g_array_append_vals (articles, &foo, 1);
				maxcnt++;
				cnt++;

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
						    COLUMN_ITEM_COVER, foo.imgfile,
						    -1);

				/* Move focus to the new row */
				path = gtk_tree_model_get_path (model, &iter);
				column = gtk_tree_view_get_column (treeview, 0);
				gtk_tree_view_set_cursor (treeview, path, column, FALSE);
				gtk_tree_path_free (path);

				// process next line
				line = strtok_r (NULL, "\n", &end_str);
			}
			// update image and statusbar
			gtk_image_set_from_file (GTK_IMAGE (a->image), foo.imgfile);
			g_snprintf (msg, 256, "# %d/%d", cnt, maxcnt);
			mid = gtk_statusbar_push (GTK_STATUSBAR (a->statusbar), id, msg);

			g_free (contents);
		}
	}
}

GtkTreeModel *
bmd_create_items_model (void)
{
	gint i = 0;
	GtkListStore *model;
	GtkTreeIter iter;

	/* create array */
	articles = g_array_sized_new (FALSE, FALSE, sizeof (Item), 1);

	//bmd_add_items(filename);

	/* create list store */
	model = gtk_list_store_new (NUM_ITEM_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
				    G_TYPE_INT, G_TYPE_STRING);

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
				    COLUMN_ITEM_COVER,
				    g_array_index (articles, Item, i).imgfile,
				    -1);
	}
	return GTK_TREE_MODEL (model);
}

void
bmd_add_item (GtkWidget *button, gpointer data)
{
	Item foo;
	GtkTreeIter current, iter;
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	bmd_widgets *a = (bmd_widgets *) data;
	GtkTreeView *treeview = (GtkTreeView *) a->treeview;

	g_return_if_fail (articles != NULL);

	cnt++;
	maxcnt++;
	foo.number = cnt;
	foo.title = g_strdup ("Title here");
	foo.author = g_strdup ("Author here");
	foo.pages = 42;
	foo.imgfile = g_strdup ("covers/dummy.png");
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
			    COLUMN_ITEM_COVER, foo.imgfile,
			    -1);

	/* Move focus to the new row */
	path = gtk_tree_model_get_path (model, &iter);
	column = gtk_tree_view_get_column (treeview, 0);
	gtk_tree_view_set_cursor (treeview, path, column, FALSE);

	gtk_tree_path_free (path);

	// update image and statusbar
	gtk_image_set_from_file (GTK_IMAGE (a->image), foo.imgfile);
	g_snprintf (msg, 256, "# %d/%d", foo.number, maxcnt);
	mid = gtk_statusbar_push (GTK_STATUSBAR (a->statusbar), id, msg);
}

void
bmd_remove_item (GtkWidget *widget, gpointer data)
{
	GtkTreeIter iter;
	bmd_widgets *a = (bmd_widgets *) data;
	GtkTreeView *treeview = (GtkTreeView *) a->treeview;
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
	cnt--;
	maxcnt--;
	g_snprintf (msg, 256, "# %d/%d", cnt, maxcnt);
	mid = gtk_statusbar_push (GTK_STATUSBAR (a->statusbar), id, msg);
}

void
bmd_cell_edited (GtkCellRendererText *cell,
		 const gchar         *path_string,
		 const gchar         *new_text,
		 gpointer             data)
{
	bmd_widgets *a = (bmd_widgets *) data;
	GtkTreeModel *model = (GtkTreeModel *) a->items_model;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
	GtkTreeIter iter;
	gchar *ifile;

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

	case COLUMN_ITEM_COVER: {
		gint i;

		i = gtk_tree_path_get_indices (path) [0];
		g_array_index (articles, Item, i).imgfile = g_strdup (new_text);

		gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
				    g_array_index (articles, Item, i).imgfile, -1);
#if DEBUG
		g_print ("%s\n", g_array_index (articles, Item, i).imgfile);
#endif
		ifile = g_array_index (articles, Item, i).imgfile;
		// update image and statusbar
		gtk_image_set_from_file (GTK_IMAGE (a->image), ifile);

	}
	break;

	}
	gtk_tree_path_free (path);
}

void
bmd_add_columns (GtkWidget *widget,
		 gpointer data)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	bmd_widgets *a = (bmd_widgets *) data;
	GtkTreeView *treeview = (GtkTreeView *) a->treeview;


	/* number column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (bmd_cell_edited), (gpointer) a);
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
			  G_CALLBACK (bmd_cell_edited), (gpointer) a);
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
			  G_CALLBACK (bmd_cell_edited), (gpointer) a);
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
			  G_CALLBACK (bmd_cell_edited), (gpointer) a);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_PAGES));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Pages", renderer,
						     "text", COLUMN_ITEM_PAGES,
						     NULL);
	// added for sorting
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 3);
	gtk_tree_view_column_set_sort_column_id (column, 3);

	/* pages column */
	renderer = gtk_cell_renderer_text_new ();
	g_object_set (renderer,
		      "editable", TRUE,
		      NULL);
	g_signal_connect (renderer, "edited",
			  G_CALLBACK (bmd_cell_edited), (gpointer) a);
	g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_COVER));

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
						     -1, "Cover", renderer,
						     "text", COLUMN_ITEM_COVER,
						     NULL);
	// added for sorting
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 4);
	gtk_tree_view_column_set_sort_column_id (column, 4);
}
/*! EOF */