/*
This file is part of andrewsmandelbrot.

Copyright (C) 2008 Andrew Boktor <andrew.boktor@gmail.com>
            
Steganography is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "utils.h"
#include <cstdlib>
#include <stdio.h>

#define XSTART -100
#define YSTART -100
#define XEND 100
#define YEND 100

#define SIZE 2000
int mandel (int x, int y) {
	float a = 0, aorg = ((float)x)/(SIZE/2), b = 0, borg = ((float)y)/(SIZE/2);
	float atmp;
	int i;
	for(i=0; i<255; i++)
	{
		atmp = a*a - b*b + aorg;
		b = 2*a*b + borg;
		a = atmp;
		if(a*a+b*b>4) return i;
	}
	return 255;
}

int main (int argc, char *argv[]) {
	BMPHeader hdr;
	hdr.bf_type = 'MB';
	hdr.bf_size = SIZE*SIZE+54;			// Need to adjust this
	hdr.bf_reserved1 = 0;
	hdr.bf_reserved2 = 0;
	hdr.bf_Off_Bits = 54;				// Need to adjust this

	printf ("###%x###\n",hdr);
	printf ("sizeof (hdr): %d\n", sizeof(BMPHeader));
	
	BMPInfoHeader info_hdr;
	info_hdr.bi_size = 40; 			// Need to adjust this
    info_hdr.bi_width = SIZE;
    info_hdr.bi_height = SIZE;
    info_hdr.bi_planes = 1;
    info_hdr.bi_bit_count = 32; 
    info_hdr.bi_compression = 0;
    info_hdr.bi_size_image = SIZE*SIZE;
    info_hdr.bixPelsPerMeter = 2835;
    info_hdr.biyPelsPerMeter = 2835;
    info_hdr.biclrUsed = 0;
    info_hdr.biclrImportant = 0;
	
	uchar4 *bmp = (uchar4 *) malloc (SIZE*SIZE*sizeof(uchar4));
	
	int mandelv;
	char r,g,b;
	
	printf ("Mandel: %d\n", mandelv);
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			mandelv = mandel (j-(SIZE/1.3), i-(SIZE/2));
			r = (mandelv & 0x7)*25;
			g = ((mandelv/8) & 0x3)*20;
			b = ((mandelv/32))*20;
			bmp[i*SIZE+j].r = r;//mandelv;
			bmp[i*SIZE+j].g = g;//mandelv;
			bmp[i*SIZE+j].b = b;//mandelv;
			bmp[i*SIZE+j].a = 255;
		}
	}
	
	save_bmp_file(&bmp, &hdr, &info_hdr, "bmpfile.bmp");
}
