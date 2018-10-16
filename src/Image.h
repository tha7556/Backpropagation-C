
/**
 * An object containing information about a pixel of an Image
**/
typedef struct Pixel_t {
    float red, green, blue, alpha;
}Pixel;
/**
 * An Image object composed of Pixels
**/
typedef struct Image_t {
    Pixel **pixels; //Matrix of pixels
    int width, height;
}Image;
/**
 * Reads in an image and stores it in the image parameter
**/
void readImage(Image *image, char *path);