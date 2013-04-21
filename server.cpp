/*
  server.cpp
  This is the source file of Server class
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */
#include "server.h"
#include "funcs.h"

int yes = 1;

Server::Server()
{
	m_client = -1;
	m_listener = -1;
	m_connected = false;
}

Server::Server( char* portNumber )
{
	m_client = -1;
	m_listener = -1;
	m_connected = false;
	m_portNumber = portNumber; 
}
Server::~Server()
{

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
/*	m_sa.sa_handler = sigchld_handler;
	sigemptyset(&m_sa.sa_mask);
	m_sa.sa_flags = SA_RESTART;
	if( sigaction(SIGCHLD, &sa, NULL) == -1)
    {
		perror("Sigaction");
		exit(1);
    }
*/
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
		{
			printf("Client socket is not initialized correcly\n");
		}
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
				//string_parser( recv_data );
				// after handling message, we clear the buffer
				memset(m_recvData, 0, MAX_DATA_LENGTH*sizeof(char));
			}
		}
	}

} 
