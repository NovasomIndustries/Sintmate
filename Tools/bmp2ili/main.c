#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BMP_SIZE    32768
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

unsigned short bmpfile[MAX_BMP_SIZE];
FILE        *fpin , *fpout;

unsigned short swap_byte(unsigned short in)
{
    return (in << 8) | (in >> 8);
}

void read_and_convert(char *infile,char *outfile,char *element_name,int *dwidth,int *dheight)
{
int         read_elements,i,j,k,dataOffset,width,height;
unsigned short  bitsPerPixel,pixel;

    fpin = fopen (infile, "r");
    if ( fpin == NULL )
    {
        printf( "Unable to open %s for read\n",infile ) ;
        return;
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
    read_elements = fread(bmpfile, 2, width*height, fpin);
    printf( "Found %d %d bit pixels ( %d bytes )\n",read_elements,bitsPerPixel,read_elements*2) ;
    printf( "Found %d x %d image\n",width,height) ;
    fclose(fpin);
    fpout = fopen (outfile, "w+");
    if ( fpout == NULL )
    {
        printf( "Unable to open %s for write\n",outfile ) ;
        return;
    }

    fprintf(fpout,"/*\n");
    fprintf(fpout,"* %s\n",outfile);
    fprintf(fpout,"*  Created on: Feb 4, 2021\n");
    fprintf(fpout," *      Author: bmp2ili\n");
    fprintf(fpout," */\n");
    fprintf(fpout,"#include \"SintMate.h\"\n");
    fprintf(fpout,"uint16_t %s_width=%d;\n",element_name,width);
    fprintf(fpout,"uint16_t %s_height=%d;\n",element_name,height);
    fprintf(fpout,"uint16_t %s[%d] = \n",element_name,read_elements);
    fprintf(fpout,"{\n");

    for(i=height-1;i>=0;i--)
    {
        k = (i-1)*width;
        for(j=0;j<width;j++)
        {
            pixel = swap_byte(bmpfile[k+j]);
            fprintf(fpout,"0x%04x,",pixel);
        }
        fprintf(fpout,"\n");
    }

    fprintf(fpout,"\n};\n");
    fclose(fpout);
    *dwidth = width;
    *dheight = height;
    printf( "width %d height %d\n",width,height ) ;
    printf( "File %s written\n",outfile ) ;
}

void create_include_file(int width , int height,int buttonwidth , int buttonheight, int iconswidth , int iconsheight, int logowidth, int logoheight)
{
char        file_out[64];
int         i;

    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Inc/SintMate/Digits.h");
    fpin = fopen (file_out, "w+");
    fprintf(fpout,"/* Digits.h\n");
    fprintf(fpout,"*  Created on: Feb 4, 2021\n");
    fprintf(fpout," *      Author: bmp2ili\n");
    fprintf(fpout," */\n");
    fprintf(fpout,"#ifndef INC_SINTMATE_DIGIT_H_\n");
    fprintf(fpout,"#define INC_SINTMATE_DIGIT_H_\n\n");
    fprintf(fpout,"#include \"main.h\"\n\n");
    fprintf(fpout,"#include \"SintMate.h\"\n");
    for(i=0;i<10;i++)
    {
        fprintf(fpout,"extern uint16_t D%dred[];\n",i);
        fprintf(fpout,"extern uint16_t D%dgreen[];\n",i);
        fprintf(fpout,"extern uint16_t D%dyellow[];\n",i);
        fprintf(fpout,"extern uint16_t D%dblue[];\n",i);
        fprintf(fpout,"extern uint16_t D%dwhite[];\n",i);
    }
    fprintf(fpout,"extern uint16_t RedOFF[];\n");

    fprintf(fpout,"\n");
    fprintf(fpout,"#define  DIGIT_WIDTH   %d\n",width);
    fprintf(fpout,"#define  DIGIT_HEIGHT  %d\n",height);
    fprintf(fpout,"#define  DIGIT_SIZE    %d\n",width*height);
    fprintf(fpout,"#define  BUTTONS_WIDTH   %d\n",buttonwidth);
    fprintf(fpout,"#define  BUTTONS_HEIGHT  %d\n",buttonheight);
    fprintf(fpout,"#define  BUTTONS_SIZE  %d\n",buttonwidth*buttonheight);
    fprintf(fpout,"#define  ICONS_WIDTH   %d\n",iconswidth);
    fprintf(fpout,"#define  ICONS_HEIGHT  %d\n",iconsheight);
    fprintf(fpout,"#define  ICONS_SIZE  %d\n",iconswidth*iconsheight);
    fprintf(fpout,"#define  LOGO_WIDTH   %d\n",logowidth);
    fprintf(fpout,"#define  LOGO_HEIGHT  %d\n",logoheight);
    fprintf(fpout,"#define  LOGO_SIZE  %d\n",logowidth*logoheight);
    fprintf(fpout,"\n");
    fprintf(fpout,"extern   uint16_t Red[10][DIGIT_SIZE];\n");
    fprintf(fpout,"extern   uint16_t Green[10][DIGIT_SIZE];\n");
    fprintf(fpout,"extern   uint16_t Yellow[10][DIGIT_SIZE];\n");
    fprintf(fpout,"extern   uint16_t Blue[10][DIGIT_SIZE];\n");
    fprintf(fpout,"extern   uint16_t White[10][DIGIT_SIZE];\n");
    fprintf(fpout,"extern   uint16_t DigitOFF[DIGIT_SIZE];\n");
    fprintf(fpout,"extern   uint16_t SintmateLogo[LOGO_SIZE];\n");
    fprintf(fpout,"\n");
    fprintf(fpout,"extern   uint16_t increase[];\n");
    fprintf(fpout,"extern   uint16_t decrease[];\n");
    fprintf(fpout,"extern   uint16_t increase_disabled[];\n");
    fprintf(fpout,"extern   uint16_t decrease_disabled[];\n");
    fprintf(fpout,"extern   uint16_t home[];\n");
    fprintf(fpout,"extern   uint16_t settings[];\n");
    fprintf(fpout,"extern   uint16_t home_disabled[];\n");
    fprintf(fpout,"extern   uint16_t settings_disabled[];\n");
    fprintf(fpout,"extern   uint16_t play[];\n");
    fprintf(fpout,"extern   uint16_t stop[];\n");
    fprintf(fpout,"\n");
    fprintf(fpout,"extern   uint16_t Increase[];\n");
    fprintf(fpout,"extern   uint16_t Decrease[];\n");
    fprintf(fpout,"extern   uint16_t Increase_disabled[];\n");
    fprintf(fpout,"extern   uint16_t Decrease_disabled[];\n");
    fprintf(fpout,"extern   uint16_t Home[];\n");
    fprintf(fpout,"extern   uint16_t Settings[];\n");
    fprintf(fpout,"extern   uint16_t Home_disabled[];\n");
    fprintf(fpout,"extern   uint16_t Settings_disabled[];\n");
    fprintf(fpout,"extern   uint16_t play[];\n");
    fprintf(fpout,"extern   uint16_t stop[];\n");
    fprintf(fpout,"\n");
    fprintf(fpout,"extern   uint16_t nfc_active[];\n");
    fprintf(fpout,"extern   uint16_t nfc_inactive[];\n");
    fprintf(fpout,"\n");
    fprintf(fpout,"#endif /*INC_SINTMATE_DIGIT_H_ */\n");
    fclose(fpout);
    printf( "File %s written\n",file_out ) ;

}

int main (int argc, char **argv)
{
char        file_in[128],file_out[128],element_name[128];
int         i,width,height;
int         buttonwidth , buttonheight,  iconswidth ,  iconsheight, logowidth , logoheight;
/*
    for(i=0;i<10;i++)
    {
        sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/D%dred.bmp",i);
        sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Red/D%dred.c",i);
        sprintf(element_name,"D%dred",i);
        read_and_convert(file_in,file_out,element_name,&width,&height);

        sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/D%dgreen.bmp",i);
        sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Green/D%dgreen.c",i);
        sprintf(element_name,"D%dgreen",i);
        read_and_convert(file_in,file_out,element_name,&width,&height);

        sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/D%dyellow.bmp",i);
        sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Yellow/D%dyellow.c",i);
        sprintf(element_name,"D%dyellow",i);
        read_and_convert(file_in,file_out,element_name,&width,&height);

        sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/D%dblue.bmp",i);
        sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Blue/D%dblue.c",i);
        sprintf(element_name,"D%dblue",i);
        read_and_convert(file_in,file_out,element_name,&width,&height);
    }
    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/RedOFF.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Red/RedOFF.c");
    sprintf(element_name,"RedOFF");
    read_and_convert(file_in,file_out,element_name,&width,&height);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/increase.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/increase.c");
    sprintf(element_name,"increase");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/decrease.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/decrease.c");
    sprintf(element_name,"decrease");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/increase_disabled.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/increase_disabled.c");
    sprintf(element_name,"increase_disabled");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/decrease_disabled.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/decrease_disabled.c");
    sprintf(element_name,"decrease_disabled");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/play.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/play.c");
    sprintf(element_name,"play");
    read_and_convert(file_in,file_out,element_name,&buttonwidth,&buttonheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/stop.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/stop.c");
    sprintf(element_name,"stop");
    read_and_convert(file_in,file_out,element_name,&buttonwidth,&buttonheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/home.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/home.c");
    sprintf(element_name,"home");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/home_disabled.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/home_disabled.c");
    sprintf(element_name,"home_disabled");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/settings.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/settings.c");
    sprintf(element_name,"settings");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/settings_disabled.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Buttons/settings_disabled.c");
    sprintf(element_name,"settings_disabled");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/SintMateLogo.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/Logo/SintMateLogo.c");
    sprintf(element_name,"SintMateLogo");
    read_and_convert(file_in,file_out,element_name,&logowidth,&logoheight);
*/
    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/nfc_inactive.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/nfc_inactive.c");
    sprintf(element_name,"nfc_inactive");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/nfc_active.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/nfc_active.c");
    sprintf(element_name,"nfc_active");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/nfc_fail.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/nfc_fail.c");
    sprintf(element_name,"nfc_fail");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

    sprintf(file_in,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/SrcImages/nfc_ok.bmp");
    sprintf(file_out,"/Devel/Stm32Sw/SintMate/Core/Src/SintMate/Images/nfc_ok.c");
    sprintf(element_name,"nfc_ok");
    read_and_convert(file_in,file_out,element_name,&iconswidth,&iconsheight);

  //  create_include_file(width , height, buttonwidth, buttonheight,iconswidth,iconsheight,logowidth,logoheight);
    return 0;
}
