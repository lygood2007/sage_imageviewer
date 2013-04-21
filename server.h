/*
  server.h
  This is the header file of the class Server.
  Network is responsible for all the issues about network configuration in Server
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#ifndef NETWORK_H
#define NETWORK_H

/*
  Server includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

#include "types.h"
#define BACKLOG 10 // Max 10 connection


class Server
{
public:
	Server();
	Server( char* portNumber );
	~Server();
	//static void* getInAddr( struct sockaddr *sa );
	void initNetwork();
	void listenClient();

private:
	int m_fdmax;
	int m_client;
	int m_listener;
	bool m_connected;
	struct sockaddr_storage m_clientAddr;
	struct sigaction m_sa;

	char m_clientIP[INET6_ADDRSTRLEN];
	char m_recvData[MAX_DATA_LENGTH];
	fd_set m_readFds;
	char* m_portNumber;

};
#endif
