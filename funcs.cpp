/*
  funcs.cpp
  This is the source file of all usefull independent functions
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#include "funcs.h"

void *get_in_addr( struct sockaddr *sa )
{
	if( sa->sa_family == AF_INET)
    {
		return &(((struct sockaddr_in*)sa)->sin_addr);
    }
	return &((struct sockaddr_in6*)sa)->sin6_addr;
}

void sigchld_handler(int s)
{
	while(waitpid(-1,NULL,WNOHANG) > 0);
}
