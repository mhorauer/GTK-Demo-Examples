/**
 * Callbacks for Classic GTK+ Menu Demo using a GtkApplication class
 *
 * M. Horauer
 */

#ifndef _bmd_menucallbacks_
#define _bmd_menucallbacks_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

/***************************************************************** PROTOTYPES */
void message_dialog (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_quit_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_open_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_save_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_saveAs_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_add_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_del_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_find_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_about_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void bmd_help_callback (GSimpleAction *action, GVariant *parameter, gpointer data);

#endif