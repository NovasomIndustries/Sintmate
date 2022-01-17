#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "ImageUploader.h"


#define MAX_BMP_SIZE    131072
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

#define BUF_SIZE    131072
unsigned short  bmpfile[BUF_SIZE],image[BUF_SIZE];
unsigned char   ascii_image[BUF_SIZE*4];
int             bmp_len;
FILE            *fpin;


unsigned short swap_byte(unsigned short in)
{
    return (in << 8) | (in >> 8);
}

int read_and_convert(char *infile)
{
int         i,j,k,dataOffset,width,height,pixel_index;
unsigned short  bitsPerPixel;
unsigned char   b[4];

    fpin = fopen (infile, "r");
    if ( fpin == NULL )
    {
        printf( "Unable to open %s for read\n",infile ) ;
        return -1;
    }

    fseek(fpin, BITS_PER_PIXEL_OFFSET, SEEK_SET);
    fread(&bitsPerPixel, 2, 1, fpin);
    if ( bitsPerPixel != 16 )
    {
        printf("Required 16 bpp image, found %d image\n",bitsPerPixel) ;
        fclose(fpin);
    }

    fseek(fpin, WIDTH_OFFSET, SEEK_SET);
    fread(&width, 4, 1, fpin);
    fseek(fpin, HEIGHT_OFFSET, SEEK_SET);
    fread(&height, 4, 1, fpin);

    fseek(fpin, DATA_OFFSET_OFFSET, SEEK_SET);
    fread(&dataOffset, 4, 1, fpin);
    fseek(fpin, dataOffset, SEEK_SET);
    bmp_len = fread(bmpfile, 2, width*height, fpin);
    //printf( "Found %d %d bit pixels ( %d bytes )\n",bmp_len,bitsPerPixel,bmp_len*2) ;
    //printf( "Found %d x %d image\n",width,height) ;
    fclose(fpin);
    pixel_index=0;
    for(i=height-1;i>=0;i--)
    {
        k = (i-1)*width;
        for(j=0;j<width;j++)
        {
            image[pixel_index] = swap_byte(bmpfile[k+j]);
            pixel_index++;
        }
    }
    pixel_index=0;

    for(i=0;i<bmp_len;i++)
    {
        k=12;
        for(j=0;j<4;j++)
        {
            b[j] = (image[i] >> k ) & 0x0f;
            if ( b[j] < 10 )
                b[j] |= 0x30;
            else
                b[j] |= 0x60;
            k -=4;
            ascii_image[pixel_index+j] = b[j];
        }
        pixel_index+=4;
    }
    return pixel_index;
}
