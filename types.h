/*
  types.h
  This is the file for all the types' definition
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <string>
using std::vector;
using std::string;
typedef unsigned char BYTE;
#define MAX_DATA_LENGTH 1024
#define TRANSFORM_MAT_LENGTH 5
/*
  Package header
 */
#define PACK_TRAN_HEAD "PACK_TRAN"
#define PACK_MESS_HEAD "PACK_MESS"
#define PACK_INIT_HEAD "PACK_INIT_POS"
#define PACK_END "END"

enum Packinfo
{
	PACK_CLOSE,
	PACK_TRANS,
	PACK_INIT
};

//#define 
//const int yes = 1;
struct IP_Pack
{
	IP_Pack(){}
	IP_Pack( string pIP, string pport) { IP = pIP; port = pport; }
	string IP;
	string port;
};

struct Config
{
	vector<IP_Pack> ipPack;
	int dimX;
	int dimY;
	int viewerWidth;
	int viewerHeight;
};

#endif
