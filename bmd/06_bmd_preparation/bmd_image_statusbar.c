/*!
 * Simple GTK Greeter Demo Application
 *
 * Version 1 - initial functional version
 * M. Horauer
 *
 * Build instructions:
 * gcc image_statusbar.c -o image_statusbar `pkg-config --cflags --libs gtk+-3.0` -Wall -g
 */
#include "bmd_image_statusbar.h"
#include "bmd_image_statusbar_callbacks.h"

guint num = 0;
gchar *msg;
gint id, mid;

const gchar ifile[CNT][16] = {"covers/0001.png",
			      "covers/0002.png",
			      "covers/0003.png",
			      "covers/0004.png",
			      "covers/0005.png",
			      "covers/0006.png",
			      "covers/0007.png",
			      "covers/0008.png",
			      "covers/0009.png",
			     };

/************************************************************* Program Layout */
void
bmd_construct_imagedemo (GtkApplication *app, GtkWidget *box, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;


	// IMAGE
	a->image = gtk_image_new_from_file (ifile[num]);
	gtk_box_pack_start (GTK_BOX (box), a->image, TRUE, TRUE, 0);
	// STATUSBAR
	a->statusbar = gtk_statusbar_new();
	gtk_box_pack_start (GTK_BOX (box), a->statusbar, FALSE, TRUE, 0);
	id = gtk_statusbar_get_context_id (GTK_STATUSBAR (a->statusbar), "default");
	msg = g_strdup_printf ("# %d/%d", num + 1, CNT);
	mid = gtk_statusbar_push (GTK_STATUSBAR (a->statusbar), id, msg);
}


#if STANDALONE
// app bmd_activate callback - creates the window
static void
bmd_activate (GtkApplication* app,
	      gpointer        user_data)
{
	GtkWidget *box;
	GtkWidget *hbox;
	GtkWidget *pbutton, *nbutton;
	bmd_widgets *a = (bmd_widgets *) user_data;

	// create the window and set a title
	a->window = gtk_application_window_new (a->app);
	gtk_window_set_title (GTK_WINDOW (a->window), "Demo");
	gtk_window_set_resizable (GTK_WINDOW (a->window), FALSE);
	gtk_window_set_default_icon_from_file ("bmd_icon.png", NULL);

	// create a grid to be used as layout container
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER (a->window), box);

	// create the inner horizontal box
	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (box), hbox, TRUE, TRUE, 0);

	// PREV button
	pbutton = gtk_button_new_with_mnemonic ("_Prev");
	gtk_box_pack_start (GTK_BOX (hbox), pbutton, TRUE, TRUE, 2);
	g_signal_connect (pbutton, "clicked", G_CALLBACK (bmd_prev_clicked), (gpointer) a);
	// NEXT button
	nbutton = gtk_button_new_with_mnemonic ("_Next");
	gtk_box_pack_start (GTK_BOX (hbox), nbutton, TRUE, TRUE, 2);
	g_signal_connect (nbutton, "clicked", G_CALLBACK (bmd_next_clicked), (gpointer) a);

	/* moved the creation of the program to a dedicated function */
	bmd_construct_imagedemo (app, GTK_WIDGET (box), (gpointer) a);

	gtk_widget_show_all (a->window);
}

int
main (int    argc,
      char **argv)
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
/** EOF */
