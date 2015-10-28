/* UW Madison, CS 559 - Fall Semester
*  By Zhouyang Michael Ye and Steve Ledvina
*  
*  Header file for sphereMesh class
*  Based upon top.h provided by Perry Kivolowitz
*/
/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for Sphere class
*/
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "meshBase.h"
#include "shader.h"


class Sphere : public MeshBase
{
public:
	Sphere();
	virtual bool Initialize(float radius, glm::vec2 dim, char * vertexShader, char* fragShader);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown(); 
	Shader starShader;
	float radius;
	float toRadians(float d);
	glm::vec4 xzRotate(vec4 pos, float offset);
	glm::vec4 yRotate(vec4 pos, float offset);

	std::vector<VertexPCNT> vertices;
	typedef MeshBase super;
};
