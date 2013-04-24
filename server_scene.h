/*
  server_scene.h
  This is the header file of the class ServerScene.
  This is only an imageviewer, the scene is just one texture
  Project:imageViewer
  Name: Yan Li
  Date: 04/21/2013
 */

#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <assert.h>
#include "types.h"
using namespace std;
class Server;

class ServerScene
{
public:
	ServerScene();
	ServerScene( Server* server );
	~ServerScene();
	void initTexture();
	void releaseTexture();
	void loadTexture( const string fileName, int index );
	void draw();

private:
	void drawQuadTexRect( const GLuint texID, const float origX, const float origY, const float width, const float height );

private:
	BYTE* m_textureBuffer[MAX_TEX_NUM]; // Texture buffer
	GLuint m_texture[MAX_TEX_NUM];

	string m_fileName;
	int m_texHeight[MAX_TEX_NUM];
	int m_texWidth[MAX_TEX_NUM];
	vector<string> m_textureNames;
	Server* m_server;
	
	// In ServerScene, I don't store the transform array because the server takes care of that 
	// Server is ServerScene's friend so this class can retrieve the private members of Server
};
#endif
