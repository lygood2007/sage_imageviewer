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

#include "types.h"
class Server;

class ServerScene
{
public:
	ServerScene();
	~ServerScene();
	void loadTexture();
	void draw();

private:
	int m_texture[1];
	Server* m_server;	
}; 
#endif
