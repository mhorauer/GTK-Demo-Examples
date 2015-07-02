/**
 * Callbacks for Book Management Demo
 *
 * M. Horauer
 */
#include "bmd_classicmenu.h"
#include "bmd_menucallbacks.h"
#include "bmd_editable_cells_callbacks.h"

/************************ MESSAGE INFO DIALOG that prints the selected action */
void
bmd_prev_clicked (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	const gchar *name;
	GtkWidget *dialog;
	bmd_widgets *a = (bmd_widgets *) data;

	name = g_action_get_name (G_ACTION (action));
	dialog = gtk_message_dialog_new (GTK_WINDOW (a->window),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_MESSAGE_INFO,
					 GTK_BUTTONS_CLOSE,
					 "Action %s was selected!",
					 name);
	g_signal_connect (dialog, "response",
			  G_CALLBACK (gtk_widget_destroy), NULL);
	gtk_widget_show (dialog);
}

/****************************************************************** CALLBACKS */
void
bmd_quit_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	bmd_save_callback (NULL, NULL, (gpointer) a);
	g_application_quit (G_APPLICATION (a->app));
}

void
bmd_open_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	GFile *file;
	bmd_widgets *a = (bmd_widgets *) data;

	GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Load Book Management Library",
							 GTK_WINDOW (a->window),
							 GTK_FILE_CHOOSER_ACTION_OPEN,
							 "_OK", GTK_RESPONSE_ACCEPT,
							 "_Cancel", GTK_RESPONSE_CANCEL,
							 NULL);
	gtk_file_filter_set_name (filter, "*.csv, *.txt");
	// I used "xdg-mime query default application/csv" to determine whether a CSV
	// mime-type is available
	gtk_file_filter_add_mime_type (filter, "text/csv");
	gtk_file_filter_add_mime_type (filter, "application/csv");
	gtk_file_filter_add_mime_type (filter, "text/x-csv");
	gtk_file_filter_add_mime_type (filter, "application/x-extension-txt");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
				     filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog))) {
	case GTK_RESPONSE_ACCEPT: {
		file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
		a->filename = g_file_get_parse_name (file);
#if DEBUG
		g_print ("Filename: %s\n", a->filename);
#endif
		bmd_add_items (file, (gpointer) a);
		break;
	}
	default:
		break;
	}
	gtk_widget_destroy (dialog);
}

void
bmd_save_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	if (a->filename == NULL)
		bmd_saveAs_callback (NULL, NULL, (gpointer) a);
	else
		bmd_save_items (a->filename, (gpointer) a);
}

void
bmd_saveAs_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Save Book Management Library",
							 GTK_WINDOW (a->window),
							 GTK_FILE_CHOOSER_ACTION_SAVE,
							 "_OK", GTK_RESPONSE_ACCEPT,
							 "_Cancel", GTK_RESPONSE_CANCEL,
							 NULL);
	gtk_file_filter_set_name (filter, "*.csv, *.txt");
	// I used "xdg-mime query default application/csv" to determine whether a CSV
	// mime-type is available
	gtk_file_filter_add_mime_type (filter, "text/csv");
	gtk_file_filter_add_mime_type (filter, "application/csv");
	gtk_file_filter_add_mime_type (filter, "text/x-csv");
	gtk_file_filter_add_mime_type (filter, "application/x-extension-txt");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
				     filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog))) {
	case GTK_RESPONSE_ACCEPT: {
		a->filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
#if DEBUG
		g_print ("Filename: %s\n", a->filename);
#endif
		bmd_save_items (a->filename, (gpointer) a);
		break;
	}
	default:
		break;
	}
	gtk_widget_destroy (dialog);
}

void
bmd_add_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	bmd_add_item (NULL, (gpointer) a);
}

void
bmd_del_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	bmd_remove_item (NULL, (gpointer) a);
}

void
bmd_find_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	bmd_prev_clicked (action, NULL, (gpointer) a);
	g_print ("Find was clicked ...\n");
}

void
bmd_about_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	bmd_prev_clicked (action, NULL, (gpointer) a);
	g_print ("About was clicked ...\n");
}

void
bmd_help_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;

	bmd_prev_clicked (action, NULL, (gpointer) a);
	g_print ("Help was clicked ...\n");
}

/** EOF */
