/*
  client.cpp
  This is the main file of client
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#include <stdio.h>
#include <stdlib.h>
#include "client.h"
//#define PORT "9000"

 int main( int argc, char** argv )
 {
	 Client client;
	 client.pushServer("127.0.0.1","9000");
	 client.pushServer("127.0.0.1","9001");
	 client.pushServer("127.0.0.1","9002");
	 client.initNetwork();
	 return 0;
 }
