/*!
 * Simple GTK Greeter Demo Application
 *
 * Version 1 - initial functional version
 * M. Horauer
 *
 * Build instructions:
 * gcc hello_gtk_v1.c `pkg-config --cflags --libs gtk+-3.0` -o welcome -Wall -g
 */
#include <gtk/gtk.h>
#include <string.h>

// globals
GtkWidget *label_output;
GtkWidget *input_entry;

// callback executed when the "Okay" button is clicked
static void
ok_clicked(GtkWidget *widget, gpointer data)
{
  gchar *buffer1, *buffer2;

  // obtain text from the entry box
  buffer1 = (gchar*)gtk_entry_get_text(GTK_ENTRY(input_entry));
  // allocate memory for the final text
  buffer2 = g_malloc(sizeof(gchar)*(strlen(buffer1)+7));
  // assemble the final text
  sprintf(buffer2,"Hello %s!", buffer1);
  // write the final text to the label on top
  gtk_label_set_text(GTK_LABEL(label_output), buffer2);
  // free the memory
  g_free(buffer2);
}

// callback that is executed when the "Clear" button is clicked
static void
clr_clicked(GtkWidget *widget, gpointer data)
{
  // clear the entry box
  gtk_entry_set_text(GTK_ENTRY(input_entry),"");
  // clear the label
  gtk_label_set_text(GTK_LABEL(label_output), "Hello ?");
}

// app activate callback - creates the window
static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *label_name;
  GtkWidget *clr_button, *ok_button;

  // create the window and set a title
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Welcome");

  // create a grid to be used as layout container
  grid = gtk_grid_new();
  gtk_container_add (GTK_CONTAINER (window), grid);

  // output label
  label_output = gtk_label_new ("Hello?");
  gtk_grid_attach(GTK_GRID(grid), label_output, 0,0,2,1);

  // name label
  label_name = gtk_label_new ("Name:");
  gtk_grid_attach(GTK_GRID(grid), label_name, 0,1,1,1);

  // text entry
  input_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), input_entry, 1,1,1,1);

  // CLEAR button
  clr_button = gtk_button_new_with_mnemonic("_Clear");
  gtk_grid_attach(GTK_GRID(grid), clr_button, 0,2,1,1);
  // connect a signal when the button is clicked -> invoke clr_clicked() callback
  g_signal_connect(clr_button, "clicked", G_CALLBACK(clr_clicked), NULL);

  // OKAY button
  ok_button = gtk_button_new_with_mnemonic("_Okay");
  gtk_grid_attach(GTK_GRID(grid), ok_button, 1,2,1,1);
  // connect a signal when the button is clicked -> invoke cok_clicked() callback
  g_signal_connect(ok_button, "clicked", G_CALLBACK(ok_clicked), NULL);

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
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  // run the application -> emits an "activate" signal
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
/** EOF */