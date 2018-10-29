#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "./Image.h"
#include <math.h>

void readImage(Image *image, char *path) {
    //Checking input is not null
    if(!image) {
        printf("The Image cannot be null\n");
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
    png_struct *pngPtr;
    png_info *info;
    unsigned int width;
    unsigned int height;
    int bitDepth;
    int colorType;
    int interlaceMethod;
    int compressionMethod;
    int filterMethod;
    png_byte **rows;
    //Reading the image
    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pngPtr) {
        printf("Cannot read image at: %s\n", path);
        return;
    }
    info = png_create_info_struct(pngPtr);
    if(!info) {
        printf("Cannot read image info at: %s\n", path);
        return;
    }
    png_init_io(pngPtr, file);
    png_read_png(pngPtr, info, 0, 0);
    png_get_IHDR(pngPtr, info, &width, &height, &bitDepth, &colorType, &interlaceMethod, &compressionMethod, &filterMethod);
    rows = png_get_rows(pngPtr, info);
    int rowbytes = png_get_rowbytes(pngPtr, info);

    //Setting the pixel values
    image->pixels = (Pixel**)malloc(sizeof(Pixel*) * height);
    for(int i = 0; i < height; i++) {
        image->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * width);
        for(int j = 0, x = 0, index = 0; j < rowbytes; j++, x++) {
            if(x == 4) {
                x = 0;
                index++;
            }
            if(x == 0)
                image->pixels[i][index].red = rows[i][j];
            else if(x == 1)
                image->pixels[i][index].green = rows[i][j];
            else if(x == 2)
                image->pixels[i][index].blue = rows[i][j];
            else if(x == 3)
                image->pixels[i][index].alpha = rows[i][j];
        }
    }
    image->width = width;
    image->height = height;
    
    //Cleaning up
    fclose(file);
    png_destroy_read_struct(&pngPtr, &info, 0);
}
void generateKernel(double **kernel, int size) {
    double sigma = 1;
    double mean = size/2;
    double sum = 0.0;
    for(int i = 0; i < size; i++) {
        kernel[i] = (double*)malloc(sizeof(double)*size);
        for(int j = 0; j < size; j++) {
            kernel[i][j] = exp(-0.5 * (pow((i-mean)/sigma, 2.0) + pow((j-mean)/sigma, 2.0))) / (2 * M_PI * sigma * sigma);
            sum += kernel[i][j];
        }
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            kernel[i][j] /= sum;
        }
    }
}
void writeImage(char *filename, Image *image) {
    printf("Writing: %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    if(!fp)
        return;
    png_struct *png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png)
        return;
    png_info *info = png_create_info_struct(png);
    if(!info)
        return;
    if(setjmp(png_jmpbuf(png)))
        return;
    png_init_io(png, fp);
    png_set_IHDR(
        png,
        info,
        image->width,
        image->height,
        8, //bit
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);
    png_byte **data = (png_byte**)malloc(sizeof(png_byte*) * image->height);
    for(int y = 0; y < image->height; y++) {
        data[y] = (png_byte*)malloc(sizeof(png_byte) * image->width * 4);
        for(int x = 0; x < image->width; x++) {
            data[y][x*4] = image->pixels[y][x].red;
            data[y][x*4+1] = image->pixels[y][x].red;
            data[y][x*4+2] = image->pixels[y][x].red;
            data[y][x*4+3] = image->pixels[y][x].alpha;
        }
    }
    png_write_image(png, data);
    png_write_end(png, NULL);
    fclose(fp);
}
void applyFilter(Image *original, Image *newImage) {
    double **kernel = (double**)malloc(sizeof(double*) * 3);
    generateKernel(kernel, 3);
    for(int i = 0; i < 3; i++) {
        for(int y = 0; y < 3; y++) {
            kernel[i][y] = .11;
            printf("%f ", kernel[i][y]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < original->height; i++) {
        for(int j = 0; j < original->width; j++) {
            printf("%f ", original->pixels[i][j].red);
        }
        printf("\n");
    }
    newImage->pixels = (Pixel**)malloc(sizeof(Pixel*) * original->height);
    newImage->height = original->height;
    newImage->width = original->width;
    for(int y = 0; y < original->height; y++) {
        for(int x = 0; x < original->width; x++) {
            double avg = kernel[1][1] * original->pixels[y][x].red;
            //I'm so sorry for what follows...
            //Left
            if(x > 0)
                avg += kernel[1][0] * original->pixels[y][x-1].red;
            else
                avg += kernel[1][0] * original->pixels[y][x].red;
            //Right
            if(x < original->width - 1)
                avg += kernel[1][2] * original->pixels[y][x+1].red;
            else
                avg += kernel[1][2] * original->pixels[y][x].red;
            //Up
            if(y > 0)
                avg += kernel[0][1] * original->pixels[y-1][x].red;
            else
                avg += kernel[0][1] * original->pixels[y][x].red;
            //Down
            if(y < original->height - 1)
                avg += kernel[2][1] * original->pixels[y+1][x].red;
            else
                avg += kernel[2][1] * original->pixels[y][x].red;
            //Top left
            if(y > 0) {
                if(x > 0) 
                    avg += kernel[0][0] * original->pixels[y-1][x-1].red;
                else
                    avg += kernel[0][0] * original->pixels[y-1][x].red;
            }
            else {
                if(x > 0)
                    avg += kernel[0][0] * original->pixels[y][x-1].red;
                else
                    avg += kernel[0][0] * original->pixels[y][x].red;
            }
            //Top right
            if(y > 0) {
                if(x < original->width - 1) 
                    avg += kernel[0][2] * original->pixels[y-1][x+1].red;
                else
                    avg += kernel[0][2] * original->pixels[y-1][x].red;
            }
            else {
                if(x < original->width- 1)
                    avg += kernel[0][2] * original->pixels[y][x+1].red;
                else
                    avg += kernel[0][2] * original->pixels[y][x].red;
            }
            //Bottom left
            if(y < original->height - 1) {
                if(x > 0) 
                    avg += kernel[2][0] * original->pixels[y+1][x-1].red;
                else
                    avg += kernel[2][0] * original->pixels[y+1][x].red;
            }
            else {
                if(x > 0)
                    avg += kernel[2][0] * original->pixels[y][x-1].red;
                else
                    avg += kernel[2][0] * original->pixels[y][x].red;
            }
            //Bottom right
            if(y < original->height - 1) {
                if(x < original->width - 1) 
                    avg += kernel[2][2] * original->pixels[y+1][x+1].red;
                else
                    avg += kernel[2][2] * original->pixels[y+1][x].red;
            }
            else {
                if(x < original->width - 1)
                    avg += kernel[2][2] * original->pixels[y][x+1].red;
                else
                    avg += kernel[2][2] * original->pixels[y][x].red;
            }
            printf("%.2f ", avg);
            newImage->pixels[y] = (Pixel*)malloc(sizeof(Pixel) * original->width);
            newImage->pixels[y][x].red = avg;
            newImage->pixels[y][x].green = avg;
            newImage->pixels[y][x].blue = avg;
            newImage->pixels[y][x].alpha = original->pixels[y][x].alpha;
        }
        printf("\n");
    }
}
int main() {
     char *png = "../images/marchesa.png";
    Image img;
    readImage(&img, png);
    Image noisy;
    applyFilter(&img, &noisy);
    writeImage("../Images/help.png", &noisy);
}