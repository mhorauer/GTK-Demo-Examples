/*!
 * A simple cairo application that rotates a source destination image
 * by a given angle between 0 and 90 degrees and writes the output
 * to a second file.
 *
 * Compile: 
 * gcc rcairo.c `pkg-config --cflags --libs gtk+-3.0` -Wall -pedantic -lm 
 *              -std=c99 -o rcairo
 *
 * Usage example: ./rcairo 20 rose.png output.png
 *
 * M. Horauer
 */

#include <cairo.h>
#include <gtk/gtk.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793

int main (int argc, char *argv[])
{
  int w, h, dx, dy;
  double angle;
  cairo_surface_t *image, *image2;
  cairo_t *cr;

  if (argc!=4)
  {
    g_print("Usage example: rcairo 20 rose.png output.png\n");
    exit(EXIT_FAILURE);
  }
  // currently only supports angles between 0 and 90 degrees
  assert(atoi(argv[1])>=0);
  assert(atoi(argv[1])<=90);

  // convert the rotation angle to radiants
  angle=atoi(argv[1])*PI/180;

  // read the source .png file and extract width and height
  image = cairo_image_surface_create_from_png (argv[2]);
  w = cairo_image_surface_get_width (image);
  h = cairo_image_surface_get_height (image);

  // calculate the new width and height of the destination image
  dx = (int)(fabs(h*cos(angle)+w*sin(angle)));
  dy = (int)(fabs(h*sin(angle)+w*cos(angle)));

  // obtain a surface where to place the destination image
  image2 = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, dy,dx);
  // obtain a cairo context
  cr = cairo_create (image2);

  // rotate the image around the top left edge
  cairo_rotate (cr, angle);
  // the user coordinate system aligns with the rotated image
  //
  cairo_translate (cr, h*cos(angle)*sin(angle), -h*sin(angle)*sin(angle));

  // create a pattern from a surface
  cairo_set_source_surface (cr, image, 0, 0);
  // perform the actual drawing
  cairo_paint (cr);
  // write the new image to the output file
  cairo_surface_write_to_png (image2, argv[3]);

  // free the surfaces
  cairo_surface_destroy (image);
  cairo_surface_destroy (image2);
  return 0;
}
/*! EOF */
