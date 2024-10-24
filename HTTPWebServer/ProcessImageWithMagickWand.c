
#include <stdio.h>
#include <stdlib.h>
#include "ProcessImageWithMagickWand.h"

//Bu classı yazarken özellikle yararlandığım siteler şunlar:
//https://imagemagick.org/api/magick-wand.php
//https://imagemagick.org/api/magick-image.php

void resizee_image(char *input_path, char *output_path, int width, int height) {

    MagickWandGenesis();
    //MagickWandGenesis() initializes the MagickWand environment.

    MagickWand *wand = NewMagickWand();
    //Sitesinde şu şekilde yazıyor:
    //NewMagickWand() returns a wand required for all other methods in the API.
    //A fatal exception is thrown if there is not enough memory to allocate the wand.
    //Use DestroyMagickWand() to dispose of the wand when it is no longer needed.

    MagickReadImage(wand, input_path);

    MagickAdaptiveResizeImage(wand, width, height);

    MagickWriteImage(wand, output_path);
    //MagickWriteImage() writes an image to the specified filename.
    //If the filename parameter is NULL, the image is written to the filename set by MagickReadImage() or
    //MagickSetImageFilename().

    DestroyMagickWand(wand); //Destroy etmeye yarıyor yani deallocate ediyor.

    MagickWandTerminus(); //MAgickwand sitesine göre bu çağrılıp magickwand ortamının silinmesi gerekiyor.

}

void crop_image(char *input_path, char *output_path, int width, int height, int x_offset, int y_offset){

    MagickWandGenesis();
    //MagickWandGenesis() initializes the MagickWand environment.

    MagickWand *wand = NewMagickWand();

    MagickReadImage(wand, input_path);

    MagickCropImage(wand, width, height, x_offset, y_offset);

    MagickWriteImage(wand, output_path);

    DestroyMagickWand(wand); //Destroy etmeye yarıyor yani deallocate ediyor.

    MagickWandTerminus();

}

void grayscale_image(char *input_path, char *output_path){

    MagickWandGenesis();
    //MagickWandGenesis() initializes the MagickWand environment.

    MagickWand *wand = NewMagickWand();

    MagickReadImage(wand, input_path);

    MagickTransformImageColorspace(wand, GRAYColorspace);

    MagickWriteImage(wand, output_path);

    DestroyMagickWand(wand); //Destroy etmeye yarıyor yani deallocate ediyor.

    MagickWandTerminus();

}

void rotate_image(char *input_path, char *output_path, double rotate_amount){

    MagickWandGenesis();
    //MagickWandGenesis() initializes the MagickWand environment.

    MagickWand *wand = NewMagickWand();

    MagickReadImage(wand, input_path);
    printf("\n\n\n\n\n\n\n\n\n\nHEREEEEEEEEEEEEEEEEEEEEEEEEE%f\n\n\n\n\n\n\n\n",rotate_amount);
    PixelWand *background_color = NewPixelWand();
    PixelSetColor(background_color, "white");
    MagickRotateImage(wand,background_color,rotate_amount);

    MagickWriteImage(wand, output_path);

    DestroyMagickWand(wand); //Destroy etmeye yarıyor yani deallocate ediyor.

    MagickWandTerminus();

}


