/*
  image_loader.h
  This is the header file of the image loading functions.
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "types.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <ImageMagick/wand/magick-wand.h>
using std::string;

#define ThrowWandException(wand)		\
  {						\
    char					\
      *description;				\
						\
    ExceptionType				\
      severity;					\
						    \
    description=MagickGetException(wand,&severity);			\
    (void) fprintf(stderr,"%s %s %ld %s\n",GetMagickModule(),description); \
    description=(char *) MagickRelinquishMemory(description);		\
    exit(-1);								\
  }

void getPixels(string fileName, BYTE **rgba, int &width,
			   int &height);

#endif
