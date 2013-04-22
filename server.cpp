/*
  server.cpp
  This is the source file of Server class
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */
#include "server.h"
#include "funcs.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
int yes = 1;

Server::Server()
{
	m_client = -1;
	m_listener = -1;
	m_connected = false;
	m_rank = -1;
	m_dimX = -1;
	m_dimY = -1;
	m_viewerWidth = -1;
	m_viewerHeight = -1;
	memset(m_transform, 0, sizeof(m_transform) );
	memset( m_clientIP, 0, sizeof(m_clientIP) );
	clearRecvData();
}

Server::Server( char* portNumber )
{
	m_client = -1;
	m_listener = -1;
	m_connected = false;
	m_portNumber = portNumber;
	m_rank = -1;
	m_dimX = -1;
	m_dimY = -1;
	m_viewerWidth = -1;
	m_viewerHeight = -1;
	memset(m_transform, 0, sizeof(m_transform) );
	memset( m_clientIP, 0, sizeof(m_clientIP) );
	clearRecvData();
}

Server::~Server()
{
	close(m_listener);
}

void Server::initNetwork()
{
	struct addrinfo hints, *servinfo, *p;

	FD_ZERO( &m_readFds );
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
  
	if(getaddrinfo(NULL, m_portNumber, &hints, &servinfo) != 0 )
    {
		perror("Get address info:");
		exit(1);
		//  The following is another way of printf
		//	fprintf(stderr, "Getaddrinfo: %s\n", gai_strerror(rv));
		//	return 1;
    }

	// loop through all the results and bind to the first we can
	for(p = servinfo; p!= NULL; p = p->ai_next)
    {
		if((m_listener = socket(p->ai_family, 
							p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("Server: socket");
			continue;
		}

		if(setsockopt(m_listener, SOL_SOCKET, SO_REUSEADDR,
					  &yes, sizeof(int)) == -1)
		{
			perror("Setsockopt");
			exit(1);
		}

		if( bind(m_listener, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(m_listener);
			perror("Server: bind");
			continue;
		}
		break;
    }

    // Dont' have service info
	if(p == NULL)
    {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
//		return 2;
    }

	freeaddrinfo(servinfo); // all done with this structure

	if(listen(m_listener, BACKLOG) == -1)
    {
		perror("Listen");
		exit(1);
    }

	FD_SET( m_listener,&m_readFds );
	
	m_fdmax = m_listener;
  
	// reap all dead processes (copied from other's code, 
	// don't know why)
	m_sa.sa_handler = sigchld_handler;
	sigemptyset(&m_sa.sa_mask);
	m_sa.sa_flags = SA_RESTART;
	if( sigaction(SIGCHLD, &m_sa, NULL) == -1)
    {
		perror("Sigaction");
		exit(1);
    }

	printf("Server: waiting for connections...\n");
	fflush(stdout);
}

void Server::listenClient()
{
		socklen_t sin_size;
	// If not connected, we wait for the first client
	if( m_connected == false )
	{
		//assert( fdmax == listener && "Socket is not initiliazed correctly");
		if (select(m_fdmax+1, &m_readFds, NULL, NULL, NULL) == -1)
		{
			perror("Select");
			exit(1);
		}
		if( FD_ISSET(m_fdmax,&m_readFds) )
		{
			sin_size = sizeof m_clientAddr; 
			// Create a new socket for the client
			m_client = accept(m_listener, (struct sockaddr*)&m_clientAddr, &sin_size );
			if( m_client == -1 )
			{
				perror("Accept");
			}
			else
			{
				FD_SET(m_client, &m_readFds);
				if( m_client > m_fdmax )
				{
					m_fdmax = m_client;
				}				
				printf("selectserver: new connection from %s on socket %d\n",
					   inet_ntop(m_clientAddr.ss_family,
								 get_in_addr((struct sockaddr*)&m_clientAddr),
								 m_clientIP,INET_ADDRSTRLEN),m_client);
				m_connected = true;
			}
		}
	}
	else
	{
		assert( m_client > 0 );
		//assert( client > 0 && "Client socket is not initialized correcly" );
		if( FD_ISSET( m_client,&m_readFds) )
		{
			int nbytes;
			if( (nbytes = recv(m_client, m_recvData, sizeof m_recvData, 0)) <= 0 )
			{
				// Connection is closed
				if( nbytes == 0 )
				{
					printf("server: socket %d hung up\n", m_client);
				}
				else
				{
					perror("Recv:");
				}
				close(m_client);
				FD_CLR(m_client,&m_readFds);
				// In this case, because we only have one client, the connected will
				// be set to zero
				m_connected = false;
			}
			else
			{
				fflush(stdout);
				//Test
				//printf("I got %s:\n",m_recvData);
				messageParser();
				//string_parser( recv_data );
				// after handling message, we clear the buffer
				//memset(m_recvData, 0, MAX_DATA_LENGTH*sizeof(char));
				clearRecvData();
			}
		}
	}
} 

bool Server::isInitialized()
{
	if( m_dimX > 0 && m_dimY > 0 && m_rank > 0 &&m_viewerWidth > 0 && m_viewerHeight > 0 )
		return true;
}

void Server::messageParser()
{
	// Convert to C++ form
	string message = m_recvData;
	int mSize = message.size();
	if( mSize == 0 )
	{
		//Empty
		return;
	}
	else
	{
		char_separator<char> sep(" ");
		tokenizer< char_separator<char> > tokens(message, sep);
		vector<string> texts;
		BOOST_FOREACH (const string& t, tokens) {
			texts.push_back(t);
		}
		if( texts.size() < 3 )// invalid
		{
			printf("Invalid Package, missing information!\n");
			return;
		}
		if( texts[texts.size()-1] != PACK_END )
		{
			printf("Invalid Package, no end flag!\n");
			return;
		}
		else
		{
			string subStr;
			stringstream ss;
			int len;
			if( texts[0] == PACK_INIT_HEAD)
			{
				len = mSize - strlen(PACK_INIT_HEAD) - strlen(PACK_END);
				subStr = message.substr(strlen(PACK_INIT_HEAD),len);
				ss<<subStr;
				ss>>m_rank>>m_dimX>>m_dimY>>m_viewerWidth>>m_viewerHeight>>m_origX>>m_origY;
				cout<<"I got: "<<m_rank<<" "<<m_dimX<<" "<<m_dimY<<" "<<m_viewerWidth<<" "<<m_viewerHeight<<" "<<m_origX<<" "<<m_origY<<endl;
			}
			else if( texts[0] == PACK_TRAN_HEAD )
			{
				len = mSize - strlen(PACK_TRAN_HEAD) - strlen(PACK_END);
				subStr = message.substr(strlen(PACK_TRAN_HEAD),len);
				ss<<subStr;
				ss>>m_transform[0]>>m_transform[1]>>m_transform[2]>>m_transform[3]>>m_transform[4];		
				cout<<"I got: "<<m_transform[0]<<" "<<m_transform[1]<<" "<<m_transform[2]<<" "<<m_transform[3]<<" "<<m_transform[4]<<endl;
			}
			else if( texts[0] == PACK_MESS_HEAD )
			{
				len = mSize - strlen(PACK_MESS_HEAD) - strlen(PACK_END)-2;
				subStr = message.substr(strlen(PACK_MESS_HEAD)+1,len);
				cout<<"I got:"<<subStr<<endl;
				if( subStr == "CLOSE" )
				{
					printf("Server is going to close\n");
					close(m_listener);
					exit(0);
				}
			}
		}
	}
}
