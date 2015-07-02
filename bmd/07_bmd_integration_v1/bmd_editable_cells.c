/* Tree View/Editable Cells
 *
 * This is a modified example of the "editable-cells" demo out of the
 * gtk3-demo program.
 *
 */

#include "bmd_editable_cells.h"
#include "bmd_editable_cells_callbacks.h"

GArray *articles = NULL;


/************************************************************* Program Layout */
void
bmd_construct_editablecells (GtkApplication *app, gpointer data)
{
	GtkWidget *sw;
	GtkWidget *treeview;
	GtkTreeModel *items_model;
	GtkTreeModel *numbers_model;
	bmd_widgets *a = (bmd_widgets *) data;

	sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
					     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_paned_pack1 (GTK_PANED (a->paned), sw, TRUE, FALSE);
	gtk_widget_set_size_request (sw, 530, 200);

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
	bmd_construct_editablecells (app, GTK_WIDGET (vbox), (gpointer) a);

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
	// run the application -> emits an "activate" signal
	status = g_application_run (G_APPLICATION (a->app), argc, argv);
	g_object_unref (a->app);

	g_free (a);
	return status;
}
#endif
/*! EOF */


