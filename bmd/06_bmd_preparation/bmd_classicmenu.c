/**
 * Classic GTK+ Menu Demo using a GtkApplication class
 *
 * M. Horauer
 */
#include "bmd_classicmenu.h"
#include "bmd_menucallbacks.h"

/******************************************************************** GLOBALS */


// map actions to callbacks
const GActionEntry app_entries[] = {
	{ "open", bmd_open_callback, NULL, NULL, NULL },
	{ "save", bmd_save_callback, NULL, NULL, NULL },
	{ "saveAs", bmd_saveAs_callback, NULL, NULL, NULL },
	{ "quit", bmd_quit_callback, NULL, NULL, NULL },
	{ "add", bmd_add_callback, NULL, NULL, NULL },
	{ "delete", bmd_del_callback, NULL, NULL, NULL },
	{ "find", bmd_find_callback, NULL, NULL, NULL },
	{ "about", bmd_about_callback, NULL, NULL, NULL },
	{ "help", bmd_help_callback, NULL, NULL, NULL }
};


/*********************************************************************** Menu */
void
bmd_construct_menu (GtkApplication *app, GtkWidget *box, gpointer data)
{
	GtkWidget *menubar;
	GMenu *menu, *filemenu, *editmenu, *helpmenu, *savemenu, *quitmenu;

	bmd_widgets *a = (bmd_widgets *) data;

	// define keyboard accelerators
	const gchar *open_accels[2] = { "<Ctrl>O", NULL };
	const gchar *save_accels[2] = { "<Ctrl>S", NULL };
	const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };
	const gchar *add_accels[2] = { "<Ctrl>A", NULL };
	const gchar *del_accels[2] = { "<Ctrl>D", NULL };
	const gchar *find_accels[2] = { "<Ctrl>F", NULL };
	const gchar *help_accels[2] = { "F1", NULL };

	/* map app entries to actions using the global structure */
	g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries,
					 G_N_ELEMENTS (app_entries), (gpointer) a);

	/* create the menu */
	menu = g_menu_new();

	/* create the File menu */
	filemenu = g_menu_new();
	g_menu_append (filemenu, "_Open", "app.open");
	savemenu = g_menu_new();
	g_menu_append (savemenu, "_Save", "app.save");
	g_menu_append (savemenu, "Save As ...", "app.saveAs");
	g_menu_append_section (filemenu, NULL, G_MENU_MODEL (savemenu));
	quitmenu = g_menu_new();
	g_menu_append (quitmenu, "_Quit", "app.quit");
	g_menu_append_section (filemenu, NULL, G_MENU_MODEL (quitmenu));
	g_menu_insert_submenu (menu, 0, "_File", G_MENU_MODEL (filemenu));
	g_object_unref (savemenu);
	g_object_unref (quitmenu);
	g_object_unref (filemenu);

	/* create the Edit menu */
	editmenu = g_menu_new();
	g_menu_append (editmenu, "_Find", "app.find");
	g_menu_append (editmenu, "_Add", "app.add");
	g_menu_append (editmenu, "_Delete", "app.delete");
	g_menu_append_submenu (menu, "_Edit", G_MENU_MODEL (editmenu));
	g_object_unref (editmenu);

	/* create the Help menu */
	helpmenu = g_menu_new();
	g_menu_append (helpmenu, "About", "app.about");
	g_menu_append (helpmenu, "Help", "app.help");
	g_menu_append_submenu (menu, "_Help", G_MENU_MODEL (helpmenu));
	g_object_unref (helpmenu);

	/* create a menu bar and add the above menus */
	menubar = gtk_menu_bar_new_from_model (G_MENU_MODEL (menu));
	gtk_box_pack_start (GTK_BOX (box), menubar, FALSE, FALSE, 0);

	/* connect keyboard accelerators */
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.open", open_accels);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.save", save_accels);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.quit", quit_accels);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.add", add_accels);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.delete", del_accels);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.find", find_accels);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
					       "app.help", help_accels);
}


/************************************************************ APP Main Window */
static void
bmd_activate (GtkApplication *app, gpointer data)
{
	GtkWidget *box;
	bmd_widgets *a = (bmd_widgets *) data;


	/* create a window with title, default size, icon and a box container */
	a->window = gtk_application_window_new (app);
	gtk_window_set_application (GTK_WINDOW (a->window), GTK_APPLICATION (app));
	gtk_window_set_position (GTK_WINDOW (a->window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (a->window), "Classic Menu Demo");
	gtk_window_set_default_size (GTK_WINDOW (a->window), 500, 300);
	gtk_window_set_default_icon_from_file ("bmd_icon.png", NULL);
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER (a->window), box);

	/* moved the creation of the menu to a dedicated function */
	bmd_construct_menu (app, GTK_WIDGET (box), (gpointer) a);

	gtk_widget_show_all (GTK_WIDGET (a->window));
}

/*********************************************************************** main */
int
main (int argc, char **argv)
{
	int status;
	bmd_widgets *a = g_malloc (sizeof (bmd_widgets));

	a->app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (a->app, "activate", G_CALLBACK (bmd_activate), (gpointer) a);
	status = g_application_run (G_APPLICATION (a->app), argc, argv);
	g_object_unref (a->app);

	g_free (a);
	return status;
}
/** EOF */
