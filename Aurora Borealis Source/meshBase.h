/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for MeshBase class
*/
#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertex.h"
#define _USE_MATH_DEFINES 
#include <math.h>
#include "shader.h"
#include "texture.h"


class MeshBase
{
public:
	MeshBase();
	virtual bool Initialize();
	virtual bool PostGLInitialize(GLuint * vertex_array_handle, GLuint * vertex_coordinate_handle, GLsizeiptr sz, const GLvoid * ptr);
	virtual void TakeDown();

	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0) = 0;
	virtual ~MeshBase();

	inline float toRadian(float d) {return (float)(d / 180.0f * M_PI);}
	inline float genRandPosFloat() {return (float)(std::rand() / (float)RAND_MAX);}
	inline void EnableNormals(bool drawNormals) {this -> draw_normals = drawNormals;}
	virtual void CustomEdit();
	static bool ShaderInit();
	static bool TextureInit();

	static LineShader splineShader;
	static Shader ribbonShader;
	static Shader solidShader;
	static Texture starTex;

	static GLuint texture_handle;

	bool draw_normals;

protected:

	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	GLuint normal_array_handle; 
	GLuint normal_coordinate_handle;

	bool GLReturnedError(char * s);
	std::vector<GLuint> vertex_indices;
	std::vector<VertexP> normal_vertices;
	std::vector<GLuint> normal_indices;

private:
	void InternalInitialize();
};