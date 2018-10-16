typedef struct Pixel_t {
    float red, green, blue, alpha;
}Pixel;
typedef struct Image_t {
    Pixel **pixels;
    int width, height;
}Image;

void readImage(Image *image, char *path);