#ifndef _image_statusbar_
#define _image_statusbar_

#include <gtk/gtk.h>
#include <string.h>

#define STANDALONE 1
#define CNT 9

extern guint num;
extern gchar *msg;
extern gint id;
extern gint mid;
extern const gchar ifile[CNT][16];

typedef struct {
	GtkApplication *app;
	GtkWidget *window;
	GtkWidget *image;
	GtkWidget *statusbar;
} bmd_widgets;

void
bmd_construct_imagedemo (GtkApplication *app, GtkWidget *box, gpointer data);

#endif
