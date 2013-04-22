/*
  config_loader.h
  This is the header file of the config loading functions.
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */


#ifndef CONFIG_LOADER
#define CONFIG_LOADER

#include <stdlib.h>
#include <string>
#include "types.h"
using std::string;

Config loadConfig(string fileName);

#endif
