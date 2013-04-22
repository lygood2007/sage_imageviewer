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
#include <unistd.h>
#include "server.h"
#include "server_scene.h"
#include "types.h"

//int rank;
//int dimX;
//int dimY;
//int winHeight;
//int winWidth;

#define TEST_VIEWER

char* port = "9000";
Server server;
ServerScene scene(&server);

int winHeight = 500;
int winWidth = 500;

int rank = -1;
int dimX = -1;
int dimY = -1;
int viewerWidth = -1;
int viewerHeight = -1;
int winOrigX = -1;
int winOrigY = -1;

double getTime()
{
    struct timeval tv;
		        
    gettimeofday(&tv,0);
    return (double)(tv.tv_sec - tv_start.tv_sec) + (double)(tv.tv_usec - tv_start.tv_usec) / 1000000.0;
}

void initSettingsFromServer( Server* server )
{
	const int* ss = server->getSettings();
	rank = ss[0];
	dimX = ss[1];
	dimY = ss[2];
	viewerWidth = ss[3];
	viewerHeight = ss[4];
	winOrigX = ss[5];
	winOrigY = ss[6];
}

bool checkSettings()
{
	if( rank == -1 ||
		dimX == -1 ||
		dimY == -1 ||
		viewerWidth == -1||
		viewerHeight == -1||
		winOrigX == -1 ||
		winOrigY == -1
		)
	{
		return false;
	}
}

void computeWinSize()
{
	int XStep = viewerWidth/dimX;
	int YStep = viewerHeight/dimY;
	int x = rank%dimX;
	int y = rank/dimY;
	
	winHeight = YStep;
	winWidth = XStep;
#ifdef TEST_VIEWER
	winOrigX = x*XStep;
	winOrigY = y*YStep;
#endif
}

void initNet()
{
	server = Server(port);
	server.initNetwork();
	while(1)
	{
		server.listenClient();
		if( server.isInitialized() )
		{
			initSettingsFromServer( server );
			if( checkSettings() )
			{
				computeWinSize();
				break;
			}
		}
	}
	else
	{
		return true;
	}
}

void initScene()
{
	scene.initTexture();
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glViewport(0,0, (GLsizei)winWidth, (GLsizei)winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-viewerWidth/2.0+winOrigX,viewerWidth/2.0+winOrigX,-viewerHeight/2.0+winOrigY+winOrigY,viewerHeight/2.0+OrigY);
	
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	/*
	  Viewport update
	 */
	glClear(GL_COLOR_BUFFER_BIT);
	scene.draw();
	// Sage part
	if( winWidth > 0 )
	{
/*
		GLubyte *rgbBuffer = nextBuffer(sageInf);
		glReadPixels(0,0,winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE,
					 rgbBuffer );
		swapBuffer(sageInf);
*/
	}
	glutSwapBuffers();
}

/*
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
}*/

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
	initScene();
	glutInit( &argc, argv );
	// Use double buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( winWidth, winHeight );
	glutInitWindowPosition( winOrigX, winOrigY );
	glutCreateWindow( argv[0] );

	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyhandler);
	glutIdleFunc(serverListen);
	init();
	glutMainLoop();
	return 0;
}
