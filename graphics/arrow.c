/* arrow.c
 * (c) 2001 Karel 'Clock' Kulhavy
 * This program is a part of Links project.
 * It takes a 32x? png image (truecolor one) and puts stdout a hex representation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

double r[256],g[256],b[256];
unsigned char rc[256],gc[256],bc[256];
double red_thr=130, green_thr=78, blue_thr=33;
double red_gamma=2000,green_gamma=2000,blue_gamma=2000;

void print_row_hex(FILE *f, unsigned char *buf, int n)
{
	int val1=0, val0=0, a;

	for (a=0;a<n;a++){
		val1<<=1;
		if (*buf==255&&buf[1]==255&&buf[2]==255)val1|=1;
		val0<<=1;
		if (!*buf&&!buf[1]&&!buf[2])val0|=1;
		buf+=3;
	}
	fprintf(f,"\t0x%08xU,0x%08xU,\n", val0, val1);
}

int main(int argc, char **argv)
{
	int x,y;
	unsigned char* image;
	png_structp png_ptr_read;
	png_infop info_ptr_read;
	FILE *in_file, *out_file;
	double gamma;


	in_file=fopen("arrow.png","r");
	out_file=fopen("arrow.inc","w");
	if (!(in_file&&out_file)){
		perror("arrow");
		exit(1);
	}
        png_ptr_read=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
        NULL);
        info_ptr_read=png_create_info_struct(png_ptr_read);
        png_init_io(png_ptr_read, in_file);
        png_read_info(png_ptr_read, info_ptr_read);
        x=png_get_image_width(png_ptr_read,info_ptr_read);
        y=png_get_image_height(png_ptr_read,info_ptr_read);
        if (png_get_gAMA(png_ptr_read,info_ptr_read, &gamma))
        {
                png_set_gamma(png_ptr_read, 1, gamma);
        }
        else
        {
                png_set_gamma(png_ptr_read, 1, 1.0);
        }
	png_read_update_info(png_ptr_read, info_ptr_read);
	image=malloc(3*x);
	if (!image){
		fprintf(stderr,"Not enough memory\n");
		exit(1);
	}

	fprintf(out_file,"/* Automatically generated by the program \"arrow\" (arrow.c) */\n\n");
	fprintf(out_file,"enum {arrow_width=32, arrow_height=%d, arrow_area=%d};\n",y,y*32);
	fprintf(out_file,"\nstatic const unsigned int arrow[%d]={\n",y+y);
	for (;y;y--){
		png_read_row(png_ptr_read,image,NULL);
		print_row_hex(out_file,image,x);
	}
	fprintf(out_file,"};\n");

	free(image);
	png_read_end(png_ptr_read, NULL);
	png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
	fclose(in_file);

	return 0;
}

