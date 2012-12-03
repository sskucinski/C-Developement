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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "utils.h"


// Load BMP file 
void load_bmp_file(uchar4 **dst, int *width, int *height, const char *name, BMPHeader *hdrt, BMPInfoHeader *infoHdrt){

    BMPHeader hdr; // Bit-Map file header
    BMPInfoHeader infoHdr; // Bit-Map file info header
    int x, y; // used in loops
	
    FILE *fd; // file read


    printf("Loading %s...\n", name);
    if(sizeof(uchar4) != 4){ // check for the format of 4 bytes
        printf("***Bad uchar4 size***\n");
        exit(0);
    }

    if( !(fd = fopen(name,"rb")) ){ //check that the file is opened for reading
        printf("***BMP load error: file access denied***\n");
        exit(0);
    }

    fread(&hdr, sizeof(hdr), 1, fd);
    if(hdr.bf_type != 'MB'){ // check for BM in the first 2 characters of the file which indicate that this is a Bit-Map file
        printf("***BMP load error: bad file format***\n");
        exit(0);
    }
    fread(&infoHdr, sizeof(infoHdr), 1, fd);

    if(infoHdr.bi_bit_count != 32){ //because  we use ARGB scheme, 32 bits for each pixel
        printf("***BMP load error: invalid color depth***\n");
        exit(0);
    }

    if(infoHdr.bi_compression){ //check that the image is not compressed
        printf("***BMP load error: compressed image***\n");
        exit(0);
    }

    *width  = infoHdr.bi_width; //load width
    *height = infoHdr.bi_height; //load height
    *dst    = (uchar4 *)malloc(*width * *height * 4); //create the destination buffer

    printf("BMP width: %u\n", infoHdr.bi_width); // print image width
    printf("BMP height: %u\n", infoHdr.bi_height); // print image height

    fseek(fd, hdr.bf_Off_Bits - sizeof(hdr) - sizeof(infoHdr), SEEK_CUR); //go to the start of the ARGB data

    for(y = 0; y < infoHdr.bi_height; y++){ // loop on height and width of the image to fill the destination buffer
        for(x = 0; x < infoHdr.bi_width; x++){
			(*dst)[(y * infoHdr.bi_width + x)].a = fgetc(fd);
            (*dst)[(y * infoHdr.bi_width + x)].b = fgetc(fd);
            (*dst)[(y * infoHdr.bi_width + x)].g = fgetc(fd);
            (*dst)[(y * infoHdr.bi_width + x)].r = fgetc(fd);
        }

        for(x = 0; x < (4 - (3 * infoHdr.bi_width) % 4) % 4; x++) // if any remaining bits in the row then discard them
            fgetc(fd);
    }

	memcpy (hdrt, &hdr, sizeof (hdr));
	memcpy (infoHdrt, &infoHdr, sizeof (infoHdr));

    if(ferror(fd)){ // check for unexpected Bit-Map file load error
        printf("***Unknown BMP load error.***\n");
        free(*dst);
        exit(0);
    }else
        printf("BMP file loaded successfully!\n");

    fclose(fd); // closes the file loaded 
}

// Save BMP file 
void save_bmp_file(uchar4 **data, BMPHeader *hdr, BMPInfoHeader *infoHdr, const char *filename){
    FILE *fd; // file to write to


    printf("Opening New file %s...\n", filename);
    if(sizeof(uchar4) != 4){ // check for the format of 4 bytes
        printf("**Bad pixel size**\n");
        exit(0);
    }
	
    if( !(fd = fopen(filename,"wb")) ){ //check that the file is opened for writing
        printf("**BMP load error: file access denied**\n");
		
        exit(0);
    }
	printf("File Opened Successfully...\n");
	printf("Writing BMP Header...\n");
    fwrite(hdr, sizeof(*hdr), 1, fd);
	//printf("Size when writing %d\n", sizeof(*hdr));

    printf("Wirting BMP Information Header...\n");
    fwrite(infoHdr, sizeof(*infoHdr), 1, fd);
	
    fseek(fd, hdr->bf_Off_Bits, SEEK_SET); //go to the start of the ARGB data
	printf("Writing BMP Image Data...\n");
	printf("Image Size: %dx%d\n", infoHdr->bi_width, infoHdr->bi_height);
	int x,y;
    for(y = 0; y < infoHdr->bi_height; y++){
        for(x = 0; x < infoHdr->bi_width; x++){
			fputc((*data)[(y * infoHdr->bi_width + x)].b , fd);
            fputc((*data)[(y * infoHdr->bi_width + x)].g , fd);
            fputc((*data)[(y * infoHdr->bi_width + x)].r , fd);
            fputc((*data)[(y * infoHdr->bi_width + x)].a , fd);
        }

        for(x = 0; x < (4 - (3 * infoHdr->bi_width) % 4) % 4; x++)
            fputc('\0',fd);
    }


    if(ferror(fd)){ // check for unexpected Bit-Map file load error
        printf("**Unknown BMP load error.**\n");
		exit(0);
    }else
        printf("BMP file saved successfully!\n");
	
    fclose(fd); // closes the file loaded 
}

// Loads a file, read 4 bytes
long load_file(uchar4 **dst, const char *name)
{
    FILE *fd;
    printf("Loading %s...\n", name);
    if( !(fd = fopen(name,"rb")) ){
        printf("***Load error: file access denied***\n");
        exit(0);
    }
	fseek (fd, 0, SEEK_END);
	long size = ftell (fd);
	fseek (fd, 0, SEEK_SET);
	*dst = (uchar4 *) malloc (size);
    fread(*dst, 4, size/4, fd);
    fclose(fd);
	printf("File Size: %d\n", size);
	return size;
}

// Loads a file, read 1 byte
long load_file1(char **dst, const char *name)
{
    FILE *fd;
    printf("Loading %s...\n", name);
    if( !(fd = fopen(name,"rb")) ){
        printf("***Load error: file access denied***\n");
        exit(0);
    }
	fseek (fd, 0, SEEK_END);
	long size = ftell (fd);
	fseek (fd, 0, SEEK_SET);
	*dst = (char *) malloc (size);
    fread(*dst, 1, size, fd);
    fclose(fd);
	printf("File Size: %d\n", size);
	return size;
}

// Saves a file
void save_file(char *dst, const char *name, int size)
{
    FILE *fd;
    printf("Saving %s...\n", name);
    if( !(fd = fopen(name,"wb")) ){ //check that the file is opened for writing
        printf("***Load error: file access denied***\n");
        exit(0);
    }
	
    fwrite(dst, 1, size, fd);
    fclose(fd);
}
