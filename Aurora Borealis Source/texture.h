/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for Texture class
*/
#pragma once
#undef _UNICODE
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define	BAD_IL_VALUE	IL_INVALID_VALUE
#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif
 
class Texture
{
public:
	Texture()
	{
		this->il_handle = BAD_IL_VALUE;
		this->il_texture_handle = BAD_GL_VALUE;
	}

	~Texture();
	
	bool Initialize(const char * file_name);
	void Bind(GLuint texture_unit = 0);

	ILint width;
	ILint height;
	ILint format;
	ILuint il_handle;
	GLuint il_texture_handle;
	GLvoid * data;
};
