/*
  scene_client.cpp
  This is the source file of the class ClientScene.
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#include "client_scene.h"
#include "image_loader.h"
#include <fstream>
using namespace std;
ClientScene::ClientScene()
{
	//m_textureBuffer = NULL;
	for( int i = 0; i < MAX_TEX_NUM; i++ )
	{
		m_textureBuffer[i] = NULL;
	}
	curTex = 0;
	resetTransformMat();
}

ClientScene::~ClientScene()
{
	releaseTexture();
}

void ClientScene::initTexture()
{
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

void ClientScene::releaseTexture()
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

void ClientScene::loadTexture( const string fileName,int index )
{
	//releaseTexture();

	getPixels( fileName, &m_textureBuffer[index], m_texWidth[index], m_texHeight[index] );

	assert( m_textureBuffer[index] != NULL );

	glGenTextures(1,&m_texture[index]);
	glBindTexture(GL_TEXTURE_2D,m_texture[index]);

	// We use mipmap here because it's a small viewport
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_texWidth[index], m_texHeight[index], GL_RGB, GL_UNSIGNED_BYTE, m_textureBuffer[index] );
}

void ClientScene::draw( const float origX, const float origY, const float width, const float height )
{
	// Should better clear the color buffer before drawing
	glPushMatrix();
	glTranslatef(m_transform[0],m_transform[1],0);
	glScalef(m_transform[3],m_transform[4],0);
	glRotatef(m_transform[2],0,0,1);
	drawQuadTex(m_texture[curTex],origX,origY,m_texWidth[curTex]/4,m_texHeight[curTex]/4);
	glPopMatrix();
	// In double buffer mode
}

void ClientScene::resetTransformMat()
{
	m_transform[0] = 0;
	m_transform[1] = 0;
	m_transform[2] = 0;
	m_transform[3] = 1;
	m_transform[4] = 1;
}

void ClientScene::updateTransformMat( TransformType type )
{
	switch (type)
	{
	case TRANS_UP:
	{
		m_transform[1]++;
		break;
	}
	case TRANS_DOWN:
	{
		m_transform[1]--;
		break;
	}
	case TRANS_LEFT:
	{
		m_transform[0]++;
		break;
	}
	case TRANS_RIGHT:
	{
		m_transform[0]--;
		break;
	}
	case ROTATE_CLO:
	{
		m_transform[2]++;
		break;
	}
	case ROTATE_CCLO:
	{
		m_transform[2]--;
		break;
	}
	case SCALE_UP:
	{
		if( m_transform[3] < 2 - 0.05 )
			m_transform[3] = m_transform[3] + 0.05;
		if( m_transform[4] < 2 - 0.05 )
			m_transform[4] = m_transform[4] + 0.05;
		break;
	}
	case SCALE_DOWN:
	{
		if( m_transform[3] > 0.05 )
			m_transform[3] = m_transform[3] - 0.05;
		if( m_transform[4] > 0.05 )
			m_transform[4] = m_transform[4] - 0.05;
		break;
	}
	default:
	{
		printf("Wrong transformation type");
		exit(1);
		break;
	}
	}
}

void ClientScene::drawQuadTex( const GLuint texID, const float origX, const float origY, const float width, const float height)
{
	float half_width = width/2;
	float half_height = height/2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texID);


	glBegin(GL_QUADS);
	glColor3f(1.0,1.0,1.0);
	glTexCoord2f(0.0,1.0);
	glVertex2f(-half_width,-half_height);
	glTexCoord2f(1.0,1.0);
	glVertex2f(half_width,-half_height);
	glTexCoord2f(1.0,0.0);
	glVertex2f(half_width,half_height);
	glTexCoord2f(0.0,0.0);
	glVertex2f(-half_width,half_height);
	glEnd();


	glDisable(GL_TEXTURE_2D);
}
