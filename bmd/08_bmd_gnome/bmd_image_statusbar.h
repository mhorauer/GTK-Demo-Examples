/*!
 * Add Image/statusbar to Book Management Demo
 *
 * M. Horauer
 */
#ifndef _bmd_image_statusbar_
#define _bmd_image_statusbar_

#include <gtk/gtk.h>
#include <string.h>
#include "bmd_gnomemenu.h"

extern guint num;
extern gchar *msg;
extern gint id;
extern gint mid;
extern gint cnt;
extern gint maxcnt;

void
bmd_construct_imagedemo (GtkApplication *app, gpointer data);

#endif
