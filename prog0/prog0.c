#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

void draw_square(cairo_t *cr, int Cx, int Cy, int L) {
	int step = rand()%(L);

	cairo_set_source_rgb (cr, 255, 255, 255);	// white
	cairo_move_to (cr, Cx+step, Cy);			// move to top 
	cairo_line_to (cr, Cx+L, Cy+step);			// right side
	cairo_line_to (cr, Cx+(L-step), Cy+L);		// bottom 
	cairo_line_to (cr, Cx, Cy+(L-step));		// left side
	cairo_line_to (cr, Cx+step, Cy);		
	cairo_fill (cr);							// fill the region outlined
}

void recurse_square(cairo_t *cr, int Cx, int Cy, int L, int level) {
	if (level==0) {
		draw_square(cr, Cx, Cy, L);		// draw square if lowest level
	}
	else {
		recurse_square(cr, Cx, Cy, (L/2), level-1);				// top left
		recurse_square(cr, Cx+(L/2), Cy, (L/2), level-1);		// top right
		recurse_square(cr, Cx+(L/2), Cy+(L/2), (L/2), level-1); // bottom right
		recurse_square(cr, Cx, Cy+(L/2), (L/2), level-1);		// bottom left
	}
}

int main (int argc, char *argv[])
{
	int size;
	int level; 

	printf("Enter size and level:\n");
	scanf("%d %d", &size, &level);

	// check if recursion depth too deep

	if (size/(int)pow(2,(double)level)<8) {
		printf("Warning: Increase length or decrease level");
		return 0;
	}
	else {
        cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, size, size );
        cairo_t *cr              = cairo_create (surface);

	recurse_square(cr,0,0,size,level);

	// send output to png file
        cairo_destroy (cr);
        cairo_surface_write_to_png (surface, "output.png");
        cairo_surface_destroy (surface);
        return 0;
	}
}

