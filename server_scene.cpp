/*
  server_scene.cpp
  This is the source file of the class ServerScene.
  Project:imageViewer
  Name: Yan Li
  Date: 04/21/2013
 */

#include "server_scene.h"
#include "server.h"
#include "image_loader.h"
#include <fstream>

ServerScene::ServerScene()
{
//	m_textureBuffer= NULL;
	for( int i = 0; i < MAX_TEX_NUM; i++ )
	{
		m_textureBuffer[i] = NULL;
	}
	// Point to nothing
	m_server = NULL;
}

ServerScene::ServerScene( Server* server )
{
	m_server = server;
	//m_textureBuffer = NULL;
	for( int i = 0; i < MAX_TEX_NUM; i++ )
	{
		m_textureBuffer[i] = NULL;
	}
}

ServerScene::~ServerScene()
{
	releaseTexture();

	// donot release the m_server cause it's not in heap
}

void ServerScene::initTexture()
{
//	loadTexture( DEFAULT_FILE_NAME );
		// firstly load the image names
	// Load the default image when initializing
	ifstream inFile;
	inFile.open(DEFAULT_FILE_NAME);
	if( !inFile.is_open() )
	{
		printf("Cannot find the txt of image names\n.");
		exit(1);
	}
	while( !inFile.eof() )
	{
		string tmp;
		inFile>>tmp;
		m_textureNames.push_back(tmp);
	}
	m_textureNames.pop_back();
	inFile.close();
	for( int i = 0; i < m_textureNames.size(); i++ )
	{
		loadTexture( m_textureNames[i],i );
	}
}

void ServerScene::releaseTexture()
{
	for( int i = 0; i < MAX_TEX_NUM; i++ )
	{ 
		if( m_textureBuffer[i] )
		{
			free(m_textureBuffer[i]);
			m_textureBuffer[i] = NULL;
		}
		glDeleteTextures(1,&m_texture[i]);
	}
}

void ServerScene::loadTexture( const string fileName,int index )
{
	// Load the texture
	//releaseTexture();
	getPixels( fileName, &m_textureBuffer[index], m_texWidth[index], m_texHeight[index] );

	assert( m_textureBuffer[index] != NULL );

	glGenTextures(1,&m_texture[index]);
	glBindTexture(GL_TEXTURE_RECTANGLE,m_texture[index]);

	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE,GL_TEXTURE_WRAP_S,GL_CLAMP);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, m_texWidth[index], m_texHeight[index], 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureBuffer[index] );

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
	drawQuadTexRect( m_texture[m_server->m_curTexIndex], m_server->m_origX, m_server->m_origY, m_texWidth[m_server->m_curTexIndex], m_texHeight[m_server->m_curTexIndex] );
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
	glTexCoord2f(width,height);
	glVertex2f(origX+halfWidth, origY-halfHeight);
	glTexCoord2f(width,0);
	glVertex2f(origX+halfWidth, origY+halfHeight );
	glTexCoord2f(0,0);
	glVertex2f(origX-halfWidth, origY+halfHeight );

	glEnd();

	glDisable(GL_TEXTURE_RECTANGLE);
}
