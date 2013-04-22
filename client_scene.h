/*
  scene_client.h
  This is the header file of the class ClientScene.
  Since this is just a viewer program, the scene is only composed of
  One texture and a transform matrix.
  Project:imageViewer
  Name: Yan Li
  Date: 04/20/2013
 */

#ifndef SCENE_CLIENT_H
#define SCENE_CLIENT_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include <stdlib.h>
#include <string>
#include <assert.h>
#include "types.h"

using std::string;
#define DEFAULT_FILE_NAME "test_image.jpg"

enum TransformType
{
	TRANS_UP = 0,
	TRANS_DOWN,
	TRANS_LEFT,
	TRANS_RIGHT,
	ROTATE_CLO,
	ROTATE_CCLO,
	SCALE_UP,
	SCALE_DOWN
};

class ClientScene
{
public:
	ClientScene();
	~ClientScene();
	void initTexture();
	void releaseTexture(); // release the current texture
	void loadTexture( const string m_fileName ); 

	void draw(const float origX, const float origY, const float width, const float height);
	void updateTransformMat( TransformType type );
	void resetTransformMat();
	inline float* getTransformMat(){return m_transform; }

private:

	void drawQuadTex(const int texID, const float origX, const float origY, const float width, const float height);

private:
	float m_transform[TRANSFORM_MAT_LENGTH];// The transform information: {trans_x,trans_y,rot_angle,scale_x,scale_y}
	GLuint m_texture[1];// The texture
	BYTE* m_textureBuffer; // The pixels of the texture
	string m_fileName;
	int m_texWidth; // The width of current Texture
	int m_texHeight; // The height of current Texture
};

#endif
