/*
  client.cpp
  This is the source file of Client class
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */
#include "client.h"
#include "funcs.h"

Client::Client()
{
	m_servNum = 0;
	m_toClose;
	clearSendData();
}

Client::~Client()
{

}

void Client::pushServer( string ipAddr, string portNumber )
{
	m_servIPAddr.push_back( ipAddr );
	m_portNumber.push_back(portNumber );
	m_servSocket.push_back( -1 );
	m_connected.push_back( false );
	m_servNum++;
}

void Client::initNetwork()
{
	using std::cout;
	using std::endl;
	assert( m_servNum == m_servIPAddr.size() );
	assert( m_servNum == m_servSocket.size() );
	assert( m_servNum == m_portNumber.size() );
	fprintf(stdout, "The client is trying to connect to:\n");
	for( unsigned int i = 0; i < m_servNum; i++ )
	{
		cout<<m_servIPAddr[i]<<":"<<m_portNumber[i]<<endl;		
	}
	for( unsigned int i = 0; i < m_servNum; i++ )
	{
		initOneServer( &m_servSocket[i],(char*)m_servIPAddr[i].c_str(), (char*)m_portNumber[i].c_str(), i );
	}
	for( unsigned int i = 0; i < m_servNum; i++ )
	{
		cout<<m_servSocket[i]<<endl;
		cout<<m_connected[i]<<endl;
	}
}

void Client::closeNetwork()
{
	for( unsigned int i = 0; i < m_servNum; i++ )
	{
		if( m_connected[i] )
			close(m_servSocket[i]);
	}
}

void Client::encapsulatePack( Packinfo info, float* transform, int* init, int rank )
{
	clearSendData();
	switch (info)
	{
	case PACK_CLOSE:
	{
		sprintf(m_sendData,PACK_MESS_HEAD " CLOSE " PACK_END );
		m_toClose = true;
		break;
	}
	case PACK_TRANS:
	{
		if( transform == NULL )
		{
			fprintf(stdout, "Invalid call.\n" );
			exit(1);
		}
		sprintf(m_sendData,PACK_TRAN_HEAD " %f %f %f %f %f " PACK_END, transform[0], transform[1],transform[2],
				transform[3],transform[4]);
		break;
	}
	case PACK_INIT:
	{
		if( init == NULL || rank == -1)
		{
			fprintf(stdout, "Invalid call.\n" );
			exit(1);
		}
		sprintf( m_sendData, PACK_INIT_HEAD " %d %d %d %d %d %d %d " PACK_END, rank, init[0], init[1], init[2], init[3], init[4], init[5] );
		break;
	}
	default:
	{
		fprintf(stdout, "Invalid call.\n" );
		exit(1);
	}
	}
}

void Client::sendData()
{
	for( unsigned int i = 0; i < m_servNum; i++ )
	{
		send( m_servSocket[i], m_sendData, strlen(m_sendData),0 );
	}
	if( m_toClose )
	{
		for( unsigned int i = 0; i < m_servNum; i++ )
		{
			close( m_servSocket[i] );
		}
	}
}

	void Client::sendServerInitData( const int dimX, const int dimY, const int serverWidth, const int serverHeight, int origX, int origY)
{
	for( int i = 0; i < dimY; i++ )
	{
		for( int j =0; j < dimX; j++ )
		{
			const int rank = i*dimX + j;
			if( (unsigned)rank >= m_servNum )
			{
				break;
			}
			int tmp[6];
			tmp[0] = dimX; tmp[1] = dimY; tmp[2] = serverWidth; tmp[3] = serverHeight;
			tmp[4] = origX; tmp[5] = origY;
			encapsulatePack(PACK_INIT, NULL, tmp, rank );
			send( m_servSocket[rank], m_sendData, strlen(m_sendData), 0 );
		}
	}
}

void Client::initOneServer( int *sockfd, char* ipAddr, char* portNumber, int index )
{
	struct addrinfo hints, *servinfo, *p;
	char s[INET_ADDRSTRLEN];
	int rv;

	memset(&hints, 0 ,sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if( (rv = getaddrinfo(ipAddr, portNumber, &hints, &servinfo)) != 0 )
	{
		fprintf(stderr, "getaddrinfo error when initializing %s:%s, error:%s\n",
				ipAddr, portNumber, gai_strerror(rv));
		exit(1);
	}

	for( p = servinfo; p != NULL; p = p->ai_next )
	{
		if(((*sockfd) = socket(p->ai_family, p->ai_socktype,
							p->ai_protocol)) == -1)
		{
			perror("client:socket");
			continue;
		}
		if( connect((*sockfd), p->ai_addr, p->ai_addrlen) == -1)
		{
			close(*sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	if( p == NULL )
	{
		fprintf(stderr,"socket create error when initializing %s:%s,\n",
				ipAddr,portNumber );
		freeaddrinfo(servinfo);
		m_connected[index] = false;
		return;
		//exit(1);
	}
	inet_ntop(p->ai_family,get_in_addr((struct sockaddr *)p->ai_addr),s,sizeof s);
	printf("client: connecting to %s\n", s);
	m_connected[index] = true;
	freeaddrinfo(servinfo);
}
