/*
  image_loader.cpp
  This is the source file of the image loading functions.
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#include "image_loader.h"

void getPixels(string fileName, BYTE **rgba, int &width,
	       int &height)
{
    // use ImageMagick to read all other formats
	  MagickBooleanType status;
    MagickWand *wand;

    // read file
    wand=NewMagickWand();
    status=MagickReadImage(wand, fileName.data());
    if (status == MagickFalse)
      ThrowWandException(wand);

    // get the image size
    width = MagickGetImageWidth(wand);
    height = MagickGetImageHeight(wand);

    // get the pixels
    *rgba = (BYTE*) malloc(width*height*3);
    memset(*rgba, 0, width*height*3);
#if MagickLibVersion >= 0x645
    MagickExportImagePixels(wand, 0, 0, width, height, "RGB", CharPixel, *rgba);
#else
    MagickGetImagePixels(wand, 0, 0, width, height, "RGB", CharPixel, *rgba);
#endif


	DestroyMagickWand(wand);
}
