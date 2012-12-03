// Reference:
// http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
// and Image Denoising CUDA


/*
This file is part of Steganography.

Copyright (C) 2008 Andrew Boktor <andrew.boktor@gmail.com>
            and Marleine Daoud <marleine@gmail.com>

Steganography is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

// uchar4 definition used as struct of image components (ARGB)
typedef struct{
	unsigned char r, g, b, a;
} uchar4;

// struct for Bit-Map file header
typedef struct{
    short bf_type; // must always be set to 'BM' to declare that this is a .bmp-file
    int bf_size; // specifies the size of the file in bytes
    short bf_reserved1; // must always be set to zero
    short bf_reserved2; // must always be set to zero
    int bf_Off_Bits; // specifies the offset from the beginning of the file to the bitmap data
} BMPHeader;

// struct for Bit-Map file info header
typedef struct{
    int bi_size; // specifies the size of the BITMAPINFOHEADER structure, in bytes
    int bi_width; // specifies the width of the image, in pixels
    int bi_height; // specifies the height of the image, in pixels
    short bi_planes; // specifies the number of planes of the target device, must be set to zero
    short bi_bit_count; // specifies the number of bits per pixel 
    unsigned bi_compression; //Specifies the type of compression, usually set to zero (no compression)
    unsigned bi_size_image; // specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero
    int bixPelsPerMeter; // specifies the the horizontal pixels per meter on the designated targer device, usually set to zero
    int biyPelsPerMeter; // specifies the the vertical pixels per meter on the designated targer device, usually set to zero
    int biclrUsed; // specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member
    int biclrImportant; // specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important
} BMPInfoHeader;

void load_bmp_file(uchar4 **dst, int *width, int *height, const char *name, BMPHeader *hdrt, BMPInfoHeader *infoHdrt);
void save_bmp_file(uchar4 **data, BMPHeader *hdr, BMPInfoHeader *infoHdr, const char *filename);
long load_file(uchar4 **dst, const char *name);
long load_file1(char **dst, const char *name);
void save_file(char *dst, const char *name, int size);