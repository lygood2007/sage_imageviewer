/*
  viewer_server.cpp
  This is the main file of viewer_server program
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
#include "server.h"
#include "server_scene.h"
#include "types.h"

//int rank;
//int dimX;
//int dimY;
//int winHeight;
//int winWidth;

//#define PORT "9000"

char* port = "9000";
Server server;
ServerScene scene;

int winHeight = 500;
int winWidth = 500;
int winPosX = 0;
int winPosY = 0;

int rank = -1;
int dimX = -1;
int dimY = -1;
int viewWidth = -1;
int viewHeight = -1;

void initNet()
{
	server = Server(port);
	server.initNetwork();
	while(1)
	{
		server.listenClient();
		if( server.isInitialized() )
			break;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
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

void serverListen()
{
	server.listenClient();
	glutPostRedisplay();
}

int main( int argc, char** argv )
{
	if( argc > 1 )
	{
		port = argv[1];
	}
	initNet();
	glutInit( &argc, argv );
	// Use double buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( winWidth, winHeight );
	glutInitWindowPosition( winPosX, winPosY );
	glutCreateWindow( argv[0] );

	//init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyhandler);
	glutIdleFunc(serverListen);
//	init();
	glutMainLoop();
	return 0;
}
