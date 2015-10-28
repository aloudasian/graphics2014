/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for Spline class
*/
#pragma once
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
#include "meshBase.h"

class Spline : public MeshBase
{
public:
	Spline();
	virtual bool Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	virtual vec3 Lerp(vec3 & start, vec3 & end, float & t);
	virtual void TakeDown();

//protected:
	VertexP startPoint;
	VertexP endPoint;
	VertexP ctrlPointOne;
	VertexP ctrlPointTwo;
	float t;
	float lerps;

	std::vector<VertexP> vertices;
	typedef MeshBase super;
};