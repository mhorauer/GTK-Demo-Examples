/*!
 * Add Image/statusbar to Book Management Demo
 *
 * M. Horauer
 */
#include "bmd_image_statusbar.h"

guint num = 0;
gchar *msg;
gint id, mid;

/************************************************************* Program Layout */
void
bmd_construct_imagedemo (GtkApplication *app, gpointer data)
{
	bmd_widgets *a = (bmd_widgets *) data;


	// IMAGE
	a->image = gtk_image_new_from_file ("covers/dummy.png");
	gtk_widget_set_size_request (a->image, XSIZE, 250);
	gtk_paned_pack2 (GTK_PANED (a->paned), a->image, TRUE, FALSE);
	// STATUSBAR
	a->statusbar = gtk_statusbar_new();
	gtk_box_pack_start (GTK_BOX (a->box), a->statusbar, FALSE, TRUE, 0);
	id = gtk_statusbar_get_context_id (GTK_STATUSBAR (a->statusbar), "default");
	msg = g_strdup_printf ("# %d/%d", num + 1, CNT);
	mid = gtk_statusbar_push (GTK_STATUSBAR (a->statusbar), id, msg);
}

/** EOF */