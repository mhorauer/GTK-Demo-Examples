/**
 * GNOME/GTK+ Menu Demo Application using a GtkApplication class
 *
 * M. Horauer
 */
#include "bmd_gnomemenu.h"
#include "bmd_menucallbacks.h"
#include "bmd_editable_cells.h"
#include "bmd_image_statusbar.h"

/******************************************************************** GLOBALS */

// map menu actions to callbacks
const GActionEntry app_actions[] = {
	{ "open", bmd_open_callback },
	{ "save", bmd_save_callback },
	{ "saveAs", bmd_saveAs_callback },
	{ "quit", bmd_quit_callback },
	{ "add", bmd_add_callback },
	{ "delete", bmd_del_callback },
	{ "find", bmd_find_callback },
	{ "about", bmd_about_callback },
	{ "help", bmd_help_callback }
};


/*********************************************************************** Menu */
void
bmd_construct_menu (GtkApplication *app, gpointer data)
{
	GtkWidget *headerbar;
	// the application menu displayed in the GNOME panel
	GMenu *appmenu;
	GMenu *editmenu;

	GtkWidget *openbutton;
	GtkWidget *savebutton;

	// the menu displayed as a popover below the gears button
	GMenu *gearmenu;
	GtkWidget *gearmenubutton;
	GtkWidget *gearicon;

	bmd_widgets *a = (bmd_widgets *) data;

	// define keyboard accelerators
	const gchar *open_accels[2] = { "<Ctrl>O", NULL };
	const gchar *save_accels[2] = { "<Ctrl>S", NULL };
	const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };
	const gchar *add_accels[2] = { "<Ctrl>A", NULL };
	const gchar *del_accels[2] = { "<Ctrl>D", NULL };
	const gchar *find_accels[2] = { "<Ctrl>F", NULL };
	const gchar *help_accels[2] = { "F1", NULL };

	// create and fill in the application menu in the GNOME panel
	appmenu = g_menu_new();
	g_menu_append (appmenu, "About", "app.about");
	g_menu_append (appmenu, "Help", "app.help");
	g_menu_append (appmenu, "_Quit", "app.quit");
	gtk_application_set_app_menu (GTK_APPLICATION (app), G_MENU_MODEL (appmenu));
	g_object_unref (appmenu);

	// create a headerbar
	headerbar = gtk_header_bar_new ();
	gtk_widget_show (headerbar);
	gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), "Book Management");
	gtk_header_bar_set_subtitle (GTK_HEADER_BAR (headerbar), "Simple Demo Application");
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerbar), TRUE);
	gtk_window_set_titlebar (GTK_WINDOW (a->window), headerbar);

	// create an OPEN button, add it to the headerbar and connect the callbacks
	openbutton = gtk_button_new_with_label ("Open");
	gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), openbutton);
	gtk_actionable_set_action_name (GTK_ACTIONABLE (openbutton), "app.open");

	// create the gear menu button
	gearmenubutton = gtk_menu_button_new();
	gearicon = gtk_image_new_from_icon_name ("emblem-system-symbolic",
						 GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_button_set_image (GTK_BUTTON (gearmenubutton), gearicon);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (headerbar), gearmenubutton);
	// create a menu for the gear button
	gearmenu = g_menu_new();
	g_menu_append (gearmenu, "Save As ...", "app.saveAs");
	editmenu = g_menu_new();
	g_menu_append (editmenu, "_Find", "app.find");
	g_menu_append (editmenu, "_Add", "app.add");
	g_menu_append (editmenu, "_Delete", "app.delete");
	g_menu_append_section (gearmenu, NULL, G_MENU_MODEL (editmenu));
	gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (gearmenubutton),
					G_MENU_MODEL (gearmenu));
	g_object_unref (editmenu);
	g_object_unref (gearmenu);

	// create an SAVE button, add it to the headerbar and connect the callbacks
	savebutton = gtk_button_new_with_label ("Save");
	gtk_header_bar_pack_end (GTK_HEADER_BAR (headerbar), savebutton);
	gtk_actionable_set_action_name (GTK_ACTIONABLE (savebutton), "app.save");

	// connect keyboard accelerators
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

/**************************************************************** MAIN WINDOW */
static void
bmd_activate (GtkApplication *app, gpointer data)
{

	bmd_widgets *a = (bmd_widgets *) data;

	// create a window with title, default size and icons
	a->window = gtk_application_window_new (app);
	gtk_window_set_application (GTK_WINDOW (a->window), GTK_APPLICATION (app));
	gtk_window_set_position (GTK_WINDOW (a->window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (a->window), "Book Management Demo");
	gtk_window_set_default_size (GTK_WINDOW (a->window), XSIZE, YSIZE);
	gtk_window_set_default_icon_from_file ("bmd_icon.png", NULL);
	a->box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER (a->window), a->box);


	/* moved the creation of the menu to a dedicated function */
	bmd_construct_menu (app, (gpointer) a);
	a->paned = gtk_paned_new (GTK_ORIENTATION_VERTICAL);
	gtk_box_pack_start (GTK_BOX (a->box), a->paned, TRUE, TRUE, 0);
	bmd_construct_editablecells (app, (gpointer) a);
	bmd_construct_imagedemo (app, (gpointer) a);

	gtk_widget_show_all (GTK_WIDGET (a->window));
}

/*********************************************************** STARTUP CALLBACK */
static void
bmd_startup (GApplication *app, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;
	// connect actions with callbacks
	g_action_map_add_action_entries (G_ACTION_MAP (app), app_actions,
					 G_N_ELEMENTS (app_actions), (gpointer) a);
}

/*********************************************************************** main */
int
main (int argc, char **argv)
{
	int status;
	bmd_widgets *a = g_malloc (sizeof (bmd_widgets));

	a->app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (a->app, "activate", G_CALLBACK (bmd_activate), (gpointer) a);
	g_signal_connect (a->app, "startup", G_CALLBACK (bmd_startup), (gpointer) a);
	status = g_application_run (G_APPLICATION (a->app), argc, argv);
	g_object_unref (a->app);

	g_free (a);
	return status;
}
/** EOF */
