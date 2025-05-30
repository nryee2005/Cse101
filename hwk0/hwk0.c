#include <stdio.h>
#include <cairo.h>

int main (int argc, char *argv[])
{
        cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, 512, 512 );
        cairo_t *cr              = cairo_create (surface);

	// set background to red
	cairo_set_source_rgb(cr, 1, 0 ,0);		// red
	cairo_rectangle(cr, 0, 0, 511, 511);	// create a rectangle that fills the entire box
	cairo_fill(cr);

	cairo_set_source_rgb (cr, 0, 1, 0);		// green
	cairo_move_to (cr, 255, 0);				// move to top middle
	cairo_line_to (cr, 0, 255);				// left middle
	cairo_line_to (cr, 255, 511);			// bottom middle
	cairo_line_to (cr, 511, 255);			// right middle
	cairo_line_to (cr, 255, 0);		
	cairo_fill (cr);						// fill the region outlined

	// send output to png file
        cairo_destroy (cr);
        cairo_surface_write_to_png (surface, "output.png");
        cairo_surface_destroy (surface);
        return 0;
}
