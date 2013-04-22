/*
  client.cpp
  This is the main file of client
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */
/*
  GL includes
 */
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "client.h"
#include "client_scene.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
#include "types.h"
using namespace std;
using namespace boost;
//#define PORT "9000"

const int winPosX = 0;
const int winPosY = 0;
int winWidth = 500;
int winHeight = 500;

Client client;
ClientScene scene;

/* Should load from config file then*/
int origX;
int origY;
int viewerServerWidth;
int viewerServerHeight;
int dimX;
int dimY;

vector<IP_Pack> nodes;

/**
   Change it to date driven method
 **/
#define CONFIG_FILENAME "./configure/config.txt"

bool loadConfig( string fileName )
{
	ifstream inFile;
	inFile.open( fileName.c_str() );
	if( !inFile.is_open() )
	{
		printf("Cannot open the file.\n");
		return false;
	}
	string line;
	std::getline(inFile,line);
	string subLine = line.substr( 0,strlen("<ImageViewer>") );
	// Firstly check the header
	if( subLine == "<ImageViewer>" )
	{
		while( !inFile.eof() )
		{
			string line;
			std::getline(inFile,line);

			char_separator<char> sep("\t :");
			tokenizer< char_separator<char> > tokens(line, sep);
			vector<string> texts;
			BOOST_FOREACH (const string& t, tokens) {
				texts.push_back(t);				
			}
			if( texts.size() == 0 )
				continue;
			else
			{
				stringstream ss;
				if( texts[0] == "<nodes>" )
				{					
					int nodeNum = 0;
					ss<<texts[1];
					ss>>nodeNum;
					nodes.resize(nodeNum);
					if( texts.size() != 2 + nodeNum*2 )
					{
						printf("Wrong nodes information\n");
						return false;
					}
					for( int m = 0; m < nodeNum; m++ )
					{
						
						IP_Pack newPack;
						newPack.IP = texts[2+2*m];
						newPack.port = texts[2+2*m+1];
						//ss>>newPack.IP>>newPack.port;
						//nodes.push_back(newPack);
						nodes[m] = newPack;
						cout<<"I am "<<nodes[m].IP<<nodes[m].port<<endl;
					}
				}
				else if( texts[0] == "<dim>" )
				{
					if( texts.size() != 1 + 2)
					{
						printf("Wrong dim information\n");
						return false;
					}

					ss<<texts[1]<<" "<<texts[2];
					ss>>dimX;
					ss>>dimY;
				}
				else if( texts[0] == "<size>" )
				{
					if( texts.size() != 1 + 2)
					{
						printf("Wrong size information\n");
						return false;
					}
					ss<<texts[1]<<" "<<texts[2];
					ss>>viewerServerWidth;
					ss>>viewerServerHeight;
				}
				else if( texts[0] == "<origin>" )
				{
					if( texts.size() != 1 + 2 )
					{											
						printf("Wrong origin information\n");
						return false;   
					}
					ss<<texts[1]<<" "<<texts[2];
					ss>>origX;
					ss>>origY;											
				}
				else
				{
					printf("Invalid file flag\n");
					return false;
				}
			}
		}
	}
	else
	{
		printf("Invalid file format");
		return false;
	}
	inFile.close();
	return true;
}

void initClient()
{
	if(!loadConfig(CONFIG_FILENAME))
	{
		// We use the default setting
		nodes.push_back( IP_Pack( "127.0.0.1","9000") );
		nodes.push_back( IP_Pack( "127.0.0.1","9001") );
		dimX = 2;
		dimY = 1;
		viewerServerWidth = 1000;
		viewerServerHeight = 1000;
		origX = 0;
		origY = 0;
	}
}

void initNet()
{
	for( unsigned int i = 0; i < nodes.size(); i++ )
	{
		client.pushServer( nodes[i].IP, nodes[i].port );
	}

	client.initNetwork();
	client.sendServerInitData( dimX, dimY, viewerServerWidth, viewerServerHeight, origX, origY );
}

void initScene()
{
	scene.initTexture();
}

void init()
{
	initClient();
	initNet();
	initScene();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	scene.draw( 0, 0, winWidth, winHeight );
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei)w, (GLsizei)h);
	winWidth = w;
	winHeight = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-w/2.0,w/2.0,-h/2.0,h/2.0);
	
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void keyhandler( unsigned char key, int x, int y )
{
	bool toClose = true;
	float* transMat = scene.getTransformMat();
	switch( key )
	{
	case 27:
	{
		toClose = true;
		//close the sockets
		printf("I'm triggered: esc\n");
		client.encapsulatePack( PACK_CLOSE );
		//sprintf(send_data, "The conenction is going to be closed" );
		
		break;
	}
	case 'w':
	{
		printf("I'm triggered: w\n");
		//sprintf(send_data,"PACK_TRA:up");
		scene.updateTransformMat(TRANS_UP);
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 's':
	{
		printf("I'm triggered: s\n");
		//sprintf(send_data,"test s\n");
		//sprintf(send_data,"PACK_TRA:down");
		scene.updateTransformMat(TRANS_DOWN);
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'a':
	{
		printf("I'm triggered: a\n");
		//	sprintf(send_data,"test a\n");
		//	sprintf(send_data,"PACK_TRA:left");
		scene.updateTransformMat(TRANS_LEFT);		
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'd':
	{
		printf("I'm triggered: d\n");
		//sprintf(send_data,"test d\n");
		//	sprintf(send_data,"PACK_TRA:right");
		scene.updateTransformMat(TRANS_RIGHT);
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'r':
	{
		// Rotate, angle ++
		printf("I'm triggered: r\n");
		scene.updateTransformMat(ROTATE_CCLO);
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'e':
	{
		// Rotate, angle -- 
		printf("I'm triggered: e\n");
		scene.updateTransformMat(ROTATE_CLO);
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'u':
	{
		// Scale
		printf("I'm triggered: u\n");
		scene.updateTransformMat(SCALE_UP);
	    client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'i':
	{
		// Scale
		printf("I'm triggered: i\n");
		scene.updateTransformMat(SCALE_DOWN);
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	case 'b':
	{
		// reset
		scene.resetTransformMat();
		client.encapsulatePack( PACK_TRANS, transMat );
		break;
	}
	default:
	{
		break;
	}
	}
/*	sprintf(send_data,"PACK_TRA:%f %f %f %f %f",m[0],m[1],m[2],m[3],m[4]);

	for( i = 0; i < SERVER_NUM; i++ )
	{
		send( serv_socket[i], send_data, strlen(send_data), 0);
	}
	if( 1 == to_close )
	{
		for( i = 0; i < SERVER_NUM; i++ )
		{
			close(serv_socket[i]);
		}
	}*/
	client.sendData();
	glutPostRedisplay();

	if( toClose )
	{
		client.closeNetwork();
		printf("Closing the client...\n");
		exit(0);
	}
}

int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	// Use double buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( winWidth, winHeight );
	glutInitWindowPosition( winPosX, winPosY );
	glutCreateWindow( argv[0] );
	//loadConfig();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyhandler);

	glutMainLoop();
	return 0;
}
