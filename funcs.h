/*
  funcs.h
  This is the header file of all usefull independent functions
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#ifndef FUNCS_H 
#define FUNCS_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

void *get_in_addr( struct sockaddr *sa );
void sigchld_handler(int s);

#endif
