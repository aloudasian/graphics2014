/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Basis for all meshes in the project, also initializes various shaders and textures
*/

#include <iostream>
#include <assert.h>
#include "meshBase.h"

using namespace std;
using namespace glm;

//Declares initial memory space for shaders and textures
LineShader MeshBase::splineShader;
Shader MeshBase::ribbonShader;
Shader MeshBase::solidShader;
Texture MeshBase::starTex;
GLuint MeshBase::texture_handle;

MeshBase::MeshBase()
{
	this->InternalInitialize();
}

//asserts all memory has been properly cleared
MeshBase::~MeshBase()
{
	assert(this->vertex_array_handle == GLuint(-1));
	assert(this->vertex_coordinate_handle == GLuint(-1));
	assert(this->normal_coordinate_handle == GLuint(-1));
	assert(this->normal_array_handle == GLuint(-1));
}

//Takes down mesh, shaders, and textures, removes handles
void MeshBase::TakeDown()
{
	this->normal_indices.clear();
	this->normal_vertices.clear();
	this->vertex_indices.clear();

	if(this->vertex_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->vertex_array_handle);

	if(this->vertex_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_coordinate_handle);

	if(this->normal_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->normal_array_handle);

	if(this->normal_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_coordinate_handle);

	if(this->texture_handle != GLuint(-1))
		glDeleteTextures(1, &this->texture_handle);
		
	this->splineShader.TakeDown();
	this->solidShader.TakeDown();
	this->ribbonShader.TakeDown();
	this->InternalInitialize();
}

//Generates and binds VAOs and Buffers
bool MeshBase::PostGLInitialize(GLuint * vertex_array_handle, GLuint * vertex_coordinate_handle, GLsizeiptr sz, const GLvoid * ptr)
{
	glGenVertexArrays(1, vertex_array_handle);
	glBindVertexArray(*vertex_array_handle);
	glGenBuffers(1, vertex_coordinate_handle);
	glBindBuffer(GL_ARRAY_BUFFER, *vertex_coordinate_handle);
	glBufferData(GL_ARRAY_BUFFER, sz, ptr, GL_STATIC_DRAW);
	return !this->GLReturnedError("Object::PostGLInitialize - on exit");
}

//Sets handles to bad GL values
bool MeshBase::Initialize()
{
	this->InternalInitialize();
	return true;
}

//Sets handles to bad GL values
void MeshBase::InternalInitialize()
{
	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	this->normal_array_handle = this->normal_coordinate_handle = GLuint(-1);
}

//Prints out GL errors
bool MeshBase::GLReturnedError(char * s)
{
	bool return_error = false;
#ifdef _DEBUG
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}
#endif
	return return_error;
}

void MeshBase::CustomEdit()
{
	return;
}

//Initializes shaders that are utilized for multiple instances of objects
bool MeshBase::ShaderInit()
{
	if (!splineShader.Initialize("splineShader.vert", "splineShader.frag"))
		return false;

	if (!ribbonShader.Initialize("ribbonShader.vert", "ribbonShader.frag"))
		return false;

	if (!solidShader.Initialize("solidShader.vert", "solidShader.frag"))
		return false;

	return true;
}

//Initializes texture used in skydome creation
bool MeshBase::TextureInit()
{
	if(!starTex.Initialize("sky.tga"))
	{
		return false;
	}

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, starTex.width, starTex.height, 0, starTex.format, GL_UNSIGNED_BYTE, starTex.data);

	return true;
}

  