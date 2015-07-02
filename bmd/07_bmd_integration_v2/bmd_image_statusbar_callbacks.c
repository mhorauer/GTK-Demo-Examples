/*!
 * Simple GTK Greeter Demo Application
 *
 * Version 1 - initial functional version
 * M. Horauer
 *
 * Build instructions:
 * gcc image_statusbar.c image_statusbar_callbacks.c -o image_statusbar 
 *   `pkg-config --cflags --libs gtk+-3.0` -Wall -g
 */
#include "bmd_image_statusbar.h"

// callback executed when the "Prev" button is clicked
void
bmd_prev_clicked(GtkWidget *widget, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;
  
  if (num == 0)
    num = 8;
  else
    num--;
  gtk_image_set_from_file(GTK_IMAGE(a->image),ifile[num]);
  g_snprintf(msg,256,"# %d/%d", num+1, CNT);
  mid = gtk_statusbar_push(GTK_STATUSBAR(a->statusbar),id,msg);
}

// callback that is executed when the "Next" button is clicked
void
bmd_next_clicked(GtkWidget *widget, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;
  
  if (num == 8)
    num = 0;
  else
    num++;
  gtk_image_set_from_file(GTK_IMAGE(a->image),ifile[num]);
  msg = g_strdup_printf("# %d/%d", num+1, CNT);
  mid = gtk_statusbar_push(GTK_STATUSBAR(a->statusbar),id,msg);
}

/** EOF */