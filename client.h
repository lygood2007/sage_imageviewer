/*
  client.h
  This is the header file of the class Client.
  Network is responsible for all the issues about network configuration in Client
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

/*
  Network includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

#include "types.h"
#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;

/*
  Package header
 */

class Client
{
public:
	Client();
	~Client();
	void pushServer( string ipAddr, string portNumber );
	void initNetwork();
	void closeNetwork();
	inline void clearSendData() { memset(m_sendData, 0, MAX_DATA_LENGTH*sizeof(char)); }
	inline char* getSendData() { return m_sendData; }
	inline int getServNum() { return m_servNum; }
	void encapsulatePack( Packinfo info, float* transform = NULL, int* init = NULL, int rank = -1 );
	void sendData();
	void sendServerInitData( const int dimX, const int dimY, const int serverWidth, const int serverHeight, int origX, int origY );

private:

	void initOneServer( int *sockfd, char* ip_addr, char* port_number, int index );

private:

	unsigned int m_servNum;
	vector<string> m_servIPAddr;
	vector<int> m_servSocket;	
	vector<string> m_portNumber;
	vector<bool> m_connected;
	char m_sendData[MAX_DATA_LENGTH];
	bool m_toClose;
};
