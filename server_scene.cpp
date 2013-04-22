/*
  server_scene.cpp
  This is the source file of the class ServerScene.
  Project:imageViewer
  Name: Yan Li
  Date: 04/21/2013
 */

#include "server_scene.h"

ServerScene::ServerScene()
{
	m_texturebuffer= NULL;
	// Point to nothing
	m_server = NULL;
}

ServerScene::ServerScene( Server* server )
{
	m_server = server;
	m_texturebuffer = NULL;
}

ServerScene::~ServerScene()
{
	releaseTexture();

	// donot release the m_server cause it's not in heap
}

void ServerScene::initTexture()
{
	loadTexture( DEFAULT_FILE_NAME );
}

void ServerScene::releaseTexture()
{
	if( m_textureBuffer )
	{
		free(m_textureBuffer);
		m_textureBuffer = NULL;
	}
	glDeleteTextures(1,&m_texture[0]);
}

void loadTexture( string fileName )
{
	// Load the texture
	releaseTexture();
	getPixels( fileName, &m_textureBuffer, m_texWidth, m_texHeight );

	assert( m_textureBuffer != NULL );

	glGenTextures(1,&m_texture[0]);
	glBindTexture(GL_TEXTURE_RECTANGLE,m_texture[0]);

	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_WRAP_S,GL_CLAMP);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_buffer );

}

void ServerScene::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// If m_server is not NULL, we apply the transformation
	if( m_server != NULL )
	{
		glTranslatef(m_server->m_transform[0],m_server->m_transform[1],0);
		glScalef(m_server->m_transform[3],m_server->m_transform[4],0);
		glRotatef(m_server->m_transform[2],0,0,1);
	}
	drawQuadTexRect( m_texture[0], m_origX, m_origY, m_texWidth, m_texHeight );
	glPopMatrix();
	glFinish();
}


void ServerScene::drawQuadTexRect( const GLuint texID, const float origX, const float origY, const float width, const float height )
{
	float halfHeight = height/2.f;
	float halfWidth = width/2.f;
	glEnable(GL_TEXTURE_RECTANGLE);
	glBindTexture(GL_TEXTURE_RECTANGLE,texID);

	glBegin(GL_QUADS);
	glColor3f(1.0,1.0,1.0);

	glTexCoord2f(0.f,height);
	glVertex2f(origX-halfWidth, origY-halfHeight );
	glTexCoord2f(m_texWidth,m_texHeight);
	glVertex2f(origX-halfWidth, origY-halfHeight);
	glTexCoord2f(m_texWidth,0);
	glVertex2f(origX-halfWidth, origY-halfHeight );
	glTexCoord2f(0,0);
	glVertex2f(origX-halfWidth, origY+halfHeight );
	glEnd();

	glDisable(GL_TEXTURE_RECTANGLE);
}
