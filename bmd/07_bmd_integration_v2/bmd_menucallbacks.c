/**
 * Callbacks for Classic GTK+ Menu Demo using a GtkApplication class
 *
 * M. Horauer
 */
#include "bmd_classicmenu.h"
#include "bmd_menucallbacks.h"
#include "bmd_editable_cells_callbacks.h"

/************************ MESSAGE INFO DIALOG that prints the selected action */
void
message_dialog(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  const gchar *name;
  GtkWidget *dialog;
  bmd_widgets *a = (bmd_widgets *)data;

  name = g_action_get_name (G_ACTION (action));
  dialog = gtk_message_dialog_new (GTK_WINDOW(a->window),
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
bmd_quit_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  g_print("Quit was clicked ...\n");
  g_application_quit(G_APPLICATION(a->app));
}

void
bmd_open_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  message_dialog(action, NULL, (gpointer)a);
  g_print("Open was clicked ...\n");
}

void
bmd_save_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  message_dialog(action, NULL, (gpointer)a);
  g_print("Save was clicked ...\n");
}

void
bmd_saveAs_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  message_dialog(action, NULL, (gpointer)a);
  g_print("Save As was clicked ...\n");
}

void
bmd_add_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;
  
  bmd_add_item(NULL, (gpointer)a);
}

void
bmd_del_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  bmd_remove_item(NULL, (gpointer)a);
}

void
bmd_find_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  message_dialog(action, NULL, (gpointer)a);
  g_print("Find was clicked ...\n");
}

void
bmd_about_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  message_dialog(action, NULL, (gpointer)a);
  g_print("About was clicked ...\n");
}

void
bmd_help_callback(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  bmd_widgets *a = (bmd_widgets *)data;

  message_dialog(action, NULL, (gpointer)a);
  g_print("Help was clicked ...\n");
}

/** EOF */