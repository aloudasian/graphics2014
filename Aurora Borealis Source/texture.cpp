/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Container for textures using the DevIL library, adopted from original code authored by Perry Kivolowitz
*/
#include <assert.h>
#include <iostream>
#include "texture.h"

using namespace std;

bool Texture::Initialize(const char * file_name)
{
	// We are asserting that we have not initialized this object before.
	assert(this->il_handle == BAD_IL_VALUE);

	if ((this->il_handle = ilGenImage()) == BAD_IL_VALUE)
		return false;
	ilBindImage(this->il_handle);
	if (!ilLoadImage(file_name))
	{
		cout<<"error loading image "<<file_name<<endl;
		return false;
	}

	glGenTextures(1, &this->il_texture_handle);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), this->width = ilGetInteger(IL_IMAGE_WIDTH), this->height = ilGetInteger(IL_IMAGE_HEIGHT), 0, this->format = ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, this->data = ilGetData());
	return true;
}

/*	To use with a shader, the sampler uniform must be fed the
	same paramter value as you call this function with.
*/

void Texture::Bind(GLuint texture_unit)
{
	// We are asserting that we have initialized this object before.
	assert(this->il_handle != BAD_IL_VALUE);

	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
}

/*	This class assumes that the object will be dynamically created.
	This  assumption is  demonstrated in the destructor which calls
	OpenGL. If an object  is  declared  in  the  global  score, the 
	destructor will run  after  the  OpenGL  context is  destroyed, 
	which would be an error.
*/

Texture::~Texture()
{
	if (this->il_texture_handle != BAD_GL_VALUE)
		glDeleteTextures(1, &this->il_texture_handle);
	if (this->il_handle != BAD_IL_VALUE)
		ilDeleteImage(this->il_handle);

	this->il_handle = BAD_IL_VALUE;
	this->il_texture_handle = BAD_GL_VALUE;
} 