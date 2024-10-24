
#include <stdio.h>
#include <wand/MagickWand.h>

void resizee_image(char *input_path, char *output_path, int width, int height);
void crop_image(char *input_path, char *output_path, int width, int height, int x_offset, int y_offset);
void grayscale_image(char *input_path, char *output_path);
void rotate_image(char *input_path, char *output_path, double rotate_amount);
