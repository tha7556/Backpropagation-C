#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "./Image.h"

void readImage(Image *im, char *path) {
    //Checking input is not null
    if(!im) {
        printf("The image cannot be null\n");
        return;
    }
    if(!path) {
        printf("The file path cannot be null\n");
        return;
    }
    //Opening the file
    FILE *file = fopen(path, "rb");    
    if(!file) {
        printf("Cannot find file: %s\n", path);
        return;
    }
    //Initializing variables needed for interpreting the image
    png_structp image;
    png_infop info;
    png_uint_32 width;
    png_uint_32 height;
    int bitDepth;
    int colorType;
    int interlaceMethod;
    int compressionMethod;
    int filterMethod;
    png_bytepp rows;
    //Reading the Image
    image = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!image) {
        printf("Cannot read image at: %s\n", path);
        return;
    }
    info = png_create_info_struct(image);
    if(!info) {
        printf("Cannot read image info at: %s\n", path);
        return;
    }
    png_init_io(image, file);
    png_read_png(image, info, 0, 0);
    png_get_IHDR(image, info, &width, &height, &bitDepth, &colorType, &interlaceMethod, &compressionMethod, &filterMethod);
    rows = png_get_rows(image, info);
    int rowbytes = png_get_rowbytes(image, info);

    //Setting the pixel values
    im->pixels = (Pixel**)malloc(sizeof(Pixel*) * height);
    for(int i = 0; i < height; i++) {
        im->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * width);
        for(int j = 0, x = 0, index = 0; j < rowbytes; j++, x++) {
            if(x == 4) {
                x = 0;
                index++;
            }
            if(x == 0)
                im->pixels[i][index].red = rows[i][j];
            else if(x == 1)
                im->pixels[i][index].green = rows[i][j];
            else if(x == 2)
                im->pixels[i][index].blue = rows[i][j];
            else if(x == 3)
                im->pixels[i][index].alpha = rows[i][j];
        }
    }
    im->width = width;
    im->height = height;
    
    //Cleaning up
    fclose(file);
    png_destroy_read_struct(&image, &info, 0);
}
int main() {
    char *png = "../images/X.png";
    Image img;
    readImage(&img, png);
    for(int i = 0; i < img.height; i++) {
        for(int j = 0; j < img.width; j++) {
            if(img.pixels[i][j].red < 64) 
                printf("#");
            else if(img.pixels[i][j].red < 128) 
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }


    for(int i = 0; i < img.height; i++) {
        free(img.pixels[i]);
    }
    free(img.pixels);
}