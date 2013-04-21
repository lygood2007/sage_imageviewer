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
