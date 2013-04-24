/*
  viewer_server.cpp
  This is the main file of viewer_server program
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

/*
  Sage includes!
 */
#include "libsage.h"
#include "misc.h"
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
#include "sys/time.h"
#include "server.h"
#include "server_scene.h"
#include "types.h"
#include <iostream>
#include <sstream>
using namespace std;

#define CONFIG_SAGE_FILENAME "configure/config_sage.conf"
#define TEST_VIEWER
//#define USE_SAGE
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
int xStep = -1;
int yStep = -1;
int dx;
int dy;
bool preDisplay = true;
struct timeval tv_start;

/**
   Sail object
 **/
sail sageInf;
sailConfig scfg;
GLubyte *rgbBuffer = 0;

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
	return true;
}

void computeWinSize()
{
	xStep = viewerWidth/dimX;
	yStep = viewerHeight/dimY;
	dx = rank%dimX;
	dy = rank/dimX;
	
	winHeight = yStep;
	winWidth = xStep;
#ifdef TEST_VIEWER
	
//	winOrigX = winOrigX;
//	winOrigY = winOrigY;

	cout<<"Init data:"<<rank<<" "<<dimX<<" "<<dimY;
	cout<<" "<<viewerWidth<<" "<<viewerHeight<<" ";
	cout<<winOrigX<<" "<<winOrigY<<endl;

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
			initSettingsFromServer( &server );
			if( checkSettings() )
			{
				computeWinSize();
				break;
			}
		}
	}
	int count = rank*100000;
	while( count -- > 0)
	{

	}
}

void initScene()
{
	scene.initTexture();
}

void initSage()
{
	// Initialize sage
	scfg.init(CONFIG_SAGE_FILENAME);
	scfg.winX = winOrigX+dx*xStep;
	scfg.winY = winOrigY+dy*yStep;
	scfg.resX = winWidth;
	scfg.resY = winHeight;
	scfg.winWidth = winWidth;
	scfg.winHeight = winHeight;

	 sageRect appImageMap;
	 appImageMap.left   = 0.0;
	 appImageMap.right  = 1.0;
	 appImageMap.bottom = 0.0;
	 appImageMap.top    = 1.0;
	 sagePixFmt pixelfmt = PIXFMT_888;
	 int roworder = BOTTOM_TO_TOP;
	 int frate = 60;

	scfg.pixFmt = pixelfmt;
	scfg.rowOrd = roworder;
	scfg.master = true;
	scfg.rendering = true;
	string myIP = "127.0.0.1";
	string myPort = port;
	string name = myIP + myPort;
	scfg.setAppName((char*)name.c_str());


	/*scfg.setAppName("imageviewer");
	scfg.appID = rank;
	//int resX = 200;
	//int resY = 200;
	//scfg.resX = resX;
	//scfg.resY = resY;
	scfg.winX = winOrigX;
	scfg.winY = winOrigY;
	scfg.winWidth = winWidth;
	scfg.winHeight = winHeight;
	scfg.pixFmt = PIXFMT_888;
	scfg.rowOrd = BOTTOM_TO_TOP;
	*/
	sageInf.init(scfg);
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glShadeModel(GL_SMOOTH);

	glViewport(0,0, (GLsizei)winWidth, (GLsizei)winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-viewerWidth/2.0 + dx*xStep+winOrigX,-viewerWidth/2.0+winOrigX+winWidth+dx*xStep,viewerHeight/2.0-winHeight-dy*yStep+winOrigY,viewerHeight/2.0-dy*yStep+winOrigY);
	
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
//	glutHideWindow();
	glViewport(0,0, (GLsizei)winWidth, (GLsizei)winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-viewerWidth/2.0 + dx*xStep+winOrigX,-viewerWidth/2.0+winOrigX+(1+dx)*xStep,viewerHeight/2.0-(1+dy)*yStep+winOrigY,viewerHeight/2.0-dy*yStep+winOrigY);
	
	glMatrixMode(GL_MODELVIEW);

	scene.draw();

#ifdef USE_SAGE
	// Sage part
	if( winWidth > 0 )
	{
		glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, rgbBuffer);
		sageInf.swapBuffer();
		rgbBuffer = (GLubyte*) sageInf.getBuffer();

		sageMessage msg;
		
		
		if (sageInf.checkMsg(msg, false) > 0) {
			switch (msg.getCode()) {
			case APP_QUIT : {
				sageInf.shutdown();
				exit(0);
				break;
			}
			}	
			}
		
	}
#endif
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glutReshapeWindow(winWidth,winHeight);
}

void serverListen()
{
	glutPostRedisplay();
    server.listenClient();
//	glutPostRedisplay();
}

int main( int argc, char** argv )
{
	if( argc > 1 )
	{
		port = argv[1];
	}
	initNet();
#ifdef USE_SAGE
	initSage();
#endif
	gettimeofday(&tv_start,0);
	glutInit( &argc, argv );
	// Use double buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( winWidth, winHeight );
	glutInitWindowPosition( winOrigX+dx*xStep, winOrigY+dy*yStep );
	glutCreateWindow( argv[0] );

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyhandler);
	glutIdleFunc(serverListen);
	initScene();
	init();
	//glPixelStorei(GL_PACK_ALIGNMENT,1);
#ifdef USE_SAGE
	if (rgbBuffer)
	    delete [] rgbBuffer;
	rgbBuffer = (GLubyte *)sageInf.getBuffer();
#endif
	glutMainLoop();
	return 0;
}
