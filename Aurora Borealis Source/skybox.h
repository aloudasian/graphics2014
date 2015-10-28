#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertex.h"
#define _USE_MATH_DEFINES 
/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for Skybox class
*/
#include <math.h>
#include "shader.h"
#include "meshBase.h"

class Skybox : public MeshBase
{
public:
	Skybox();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	virtual void TakeDown();

	std::vector<VertexPT> vertices;
	GLuint cubemap_handle;
	CubemapShader shader;
	Texture skybox_containers[6]; 
	std::vector<std::string> texture_names;
	
	int number_of_textures;
	typedef MeshBase super;

private:
	void PushVertices(mat4 & m);
};

