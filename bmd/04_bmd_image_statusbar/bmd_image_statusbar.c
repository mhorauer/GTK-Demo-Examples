/*!
 * Simple GTK Greeter Demo Application
 *
 * Version 1 - initial functional version
 * M. Horauer
 *
 * Build instructions:
 * gcc bmd_image_statusbar.c -o bmd `pkg-config --cflags --libs gtk+-3.0` -Wall -g
 */
#include <gtk/gtk.h>
#include <string.h>

#define CNT 9
guint num = 0;
gchar *msg;

GtkWidget *image, *statusbar;
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

// callback executed when the "Prev" button is clicked
void
prev_clicked (GtkWidget *widget, gpointer data)
{
	if (num == 0)
		num = 8;
	else
		num--;
	gtk_image_set_from_file (GTK_IMAGE (image), ifile[num]);
	g_snprintf (msg, 256, "# %d/%d", num + 1, CNT);
	mid = gtk_statusbar_push (GTK_STATUSBAR (statusbar), id, msg);
}

// callback that is executed when the "Next" button is clicked
void
next_clicked (GtkWidget *widget, gpointer data)
{
	if (num == 8)
		num = 0;
	else
		num++;
	gtk_image_set_from_file (GTK_IMAGE (image), ifile[num]);
	msg = g_strdup_printf ("# %d/%d", num + 1, CNT);
	mid = gtk_statusbar_push (GTK_STATUSBAR (statusbar), id, msg);
}

// app bmd_activate callback - creates the window
static void
bmd_activate (GtkApplication* app,
	      gpointer        user_data)
{
	GtkWidget *window;
	GtkWidget *box, *hbox;
	GtkWidget *pbutton, *nbutton;

	// create the window and set a title
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Demo");
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	gtk_window_set_default_icon_from_file ("bmd_icon.png", NULL);

	// create a grid to be used as layout container
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER (window), box);

	// create the inner horizontal box
	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (box), hbox, TRUE, TRUE, 0);

	// PREV button
	pbutton = gtk_button_new_with_mnemonic ("_Prev");
	gtk_box_pack_start (GTK_BOX (hbox), pbutton, TRUE, TRUE, 2);
	g_signal_connect (pbutton, "clicked", G_CALLBACK (prev_clicked), NULL);
	// NEXT button
	nbutton = gtk_button_new_with_mnemonic ("_Next");
	gtk_box_pack_start (GTK_BOX (hbox), nbutton, TRUE, TRUE, 2);
	g_signal_connect (nbutton, "clicked", G_CALLBACK (next_clicked), NULL);

	// IMAGE
	image = gtk_image_new_from_file (ifile[num]);
	gtk_box_pack_start (GTK_BOX (box), image, TRUE, TRUE, 0);
	// STATUSBAR
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start (GTK_BOX (box), statusbar, FALSE, TRUE, 0);
	id = gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "default");
	msg = g_strdup_printf ("# %d/%d", num + 1, CNT);
	mid = gtk_statusbar_push (GTK_STATUSBAR (statusbar), id, msg);

	gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
	GtkApplication *app;
	int status;

	// create a threaded application
	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (bmd_activate), NULL);
	// run the application -> emits an "bmd_activate" signal
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
/** EOF */
