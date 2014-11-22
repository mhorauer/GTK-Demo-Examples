/**
 * @mainpage
 * @brief A simple demo application that generates random data values using a
 *        data_thread and writes the later to a global array.
 *        A second thread takes these values and writes them to a plot.
 *
 * @aurthor M. Horauer
 * @date    2014
 * 
 * gcc -Wall `pkg-config --cflags --libs gtk+-3.0` cairoplot.c -o cairoplot 
 *     -lm -Wall --pedantic
 */

/******************************************************************* INCLUDES */
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/******************************************************************** DEFINES */
#define GRID 10
#define SAMPLES 500
#define M_PI 3.14159265358979323846

/******************************************************** FUNCTION PROTOTYPES */
static gboolean drawGrid(cairo_t * cr);
static gboolean drawDiagram(GtkWidget *widget);
static gboolean drawEvent(GtkWidget *widget, gpointer data);
static gpointer guiThread(gpointer data);
static gboolean guiHandler(GtkWidget *widget);
static gpointer dataThread(gpointer data);
static gboolean dataHandler(gpointer data);

/*********************************************************** GLOBAL VARIABLES */
/* create a mutex */
G_LOCK_DEFINE (value);

/* variables to draw the grid and the initial window */
gint xsize = 600;
gint ysize = 400;
gint xoff = 50;
gint yoff = 50;
/* global array that holds the values to be drawn */
gint valuea[SAMPLES];
gint valueb[SAMPLES];

GtkWidget* window;
GtkWidget* drawing_area;

/****************************************************************** FUNCTIONS */

/**
 * @brief draw a grid using "GRID intervals" both in x and y direction
 */
static gboolean
drawGrid (cairo_t * cr)
{
  double width, height;
  int i;
  static const double dashed1[] = {3.0, 2.0};
  static int len1  = sizeof(dashed1) / sizeof(dashed1[0]);

  height = ysize;
  width = xsize;

  for (i = 0; i<=GRID; i++) {
    /* we use the default color black */
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    /* use solid lines on the outside and dashes inside */
    if (i!=0 && i!=GRID)
    {
      cairo_set_line_width (cr, 0.1);
      cairo_set_dash(cr, dashed1, len1, 0);
    }
    else
    {
      cairo_set_line_width (cr, 1.5);
      cairo_set_dash(cr, dashed1, 0, 0);
    }
    /* draw y lines */ 
    cairo_move_to (cr, i*width/GRID+xoff/2, yoff/2);
    cairo_line_to (cr, i*width/GRID+xoff/2, height+yoff/2);
    cairo_stroke (cr);

    /* draw x lines */
    cairo_move_to (cr, xoff/2,i*height/GRID+yoff/2);
    cairo_line_to (cr, width+xoff/2,i*height/GRID+yoff/2);
    cairo_stroke (cr);
  }
  return FALSE;
}

/**
 * @brief perform the actual drawing
 */
static gboolean
drawDiagram (GtkWidget *widget)
{
  double width;
  int i;
  cairo_t* cr;

  width = xsize;
  /* create a Cairo context for drawing */
  cr = gdk_cairo_create (gtk_widget_get_window(drawing_area));
  /* draw the grid */
  drawGrid (cr);
  /* set the line width */
  cairo_set_line_width (cr, 0.75);

  /* draw the random values from valuea[] */
  /* set the drawing color */
  cairo_set_source_rgb (cr, 0.0, 0.5, 0.5);
  for (i = 0; i < SAMPLES; i++) {
    /* move to the (x= x-axis / y= current data value) */
    cairo_move_to (cr, i * (width/SAMPLES)+xoff/2,ysize-valuea[i]+yoff/2);
    /* draw a line to the next x-axis position and the next data value */
    cairo_line_to (cr,(i+1)*(width/SAMPLES)+xoff/2,ysize-valuea[i+1]+yoff/2);
    /* draw */
    cairo_stroke (cr);
  }

  /* draw a second graph with data stored in valueb[] */
  /* set the drawing color */
  cairo_set_source_rgb (cr, 0.5, 0.0, 0.5);
  for (i = 0; i < SAMPLES; i++) {
    cairo_move_to (cr, i * (width/SAMPLES)+xoff/2,ysize-valueb[i]+yoff/2);
    cairo_line_to (cr,(i+1)*(width/SAMPLES)+xoff/2,ysize-valueb[i+1]+yoff/2);
    cairo_stroke (cr);
  }

  cairo_destroy (cr);
  return FALSE;
}

/**
 * @brief redraw the graph on: resize, creation, overlap
 */
static gboolean
drawEvent(GtkWidget* widget, gpointer data)
{
  xsize = gtk_widget_get_allocated_width(widget)-xoff;
  ysize = gtk_widget_get_allocated_height(widget)-yoff;  
  drawDiagram(widget);
  return TRUE;
}

/**
 * @brief data thread invokes a data generator after a short timeout
 */
static gpointer
dataThread(gpointer data)
{
  g_timeout_add(55, (GSourceFunc)dataHandler, NULL);
  return NULL;
}

/**
 * @brief some data generation
 */
static gboolean
dataHandler(gpointer data)
{
  static gint j, fsweep=0;

  G_LOCK(value);
  fsweep++;
  for(j=0;j<=SAMPLES;j++)
    valuea[j]=g_random_int ()%ysize;
  for(j=0;j<=SAMPLES;j++)
    valueb[j]=(gint)(sin(j*M_PI/180*fsweep)*(ysize+yoff)/10)+ysize/2;
  G_UNLOCK(value);
  return TRUE;
}

/**
 * @brief GUI thread only create a plot surface and periodically calls a
 *        guiHandler that redraws the plot after a short timeout
 */
static gpointer
guiThread(gpointer data)
{
  /* create the top-level window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Cairo Plot Demo");
  gtk_window_set_default_icon_from_file("cplot.png", NULL);
  gtk_window_set_default_size(GTK_WINDOW(window), xsize+xoff, ysize+yoff);
  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);
  /* create a drawing area */
  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, xsize, ysize);
  gtk_container_add(GTK_CONTAINER (window), drawing_area);
  g_signal_connect(G_OBJECT(drawing_area), "draw",
	                 G_CALLBACK(drawEvent), NULL);
  /* periodically redraw the data */
  g_timeout_add(300, (GSourceFunc)guiHandler, (gpointer)window);
  /* */

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();
  return NULL;
}

/**
 * @brief simply redraw the drawing area
 */
static gboolean
guiHandler(GtkWidget *widget)
{
  gtk_widget_queue_draw(widget);
  return TRUE;
}

/**
 * @brief main only creates two threads
 */
int
main (int argc, char *argv[])
{
  GThread* GuiThread;
  GThread* DataThread;

  gtk_init(&argc, &argv);

  /* create two threads: one for the GUI and the DATA, respectively */
  GuiThread = g_thread_new("Thread GUI", (GThreadFunc)guiThread, (void *)NULL);
  DataThread = g_thread_new("Thread DATA", (GThreadFunc)dataThread, (void *)NULL);

  g_thread_join(DataThread);
  g_thread_join(GuiThread);
  return 0;
}
/** EOF */
