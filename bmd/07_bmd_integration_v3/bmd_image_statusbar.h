/*!
 * Add Image/statusbar to Book Management Demo
 *
 * M. Horauer
 */
#ifndef _bmd_image_statusbar_
#define _bmd_image_statusbar_

#include <gtk/gtk.h>
#include <string.h>
#include "bmd_classicmenu.h"

#define CNT 9

extern guint num;
extern gchar *msg;
extern gint id;
extern gint mid;
extern const gchar ifile[CNT][16];

void bmd_construct_imagedemo (GtkApplication *app, gpointer data);

#endif