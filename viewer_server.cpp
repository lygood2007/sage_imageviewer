/*
  viewer_server.cpp
  This is the main file of viewer_server program
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "types.h"

//int rank;
//int dimX;
//int dimY;
//int winHeight;
//int winWidth;

//#define PORT "9000"

char* port = "9000";

int main( int argc, char** argv )
{
	if( argc > 1 )
	{
		port = argv[1];
	}
	Server server(port);
	server.initNetwork();
	server.listenClient();
	return 0;
}
