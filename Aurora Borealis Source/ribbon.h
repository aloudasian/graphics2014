/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for Ribbon class
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
#include "spline.h"


class Ribbon : public Spline
{
public:
	Ribbon();
	virtual bool Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps, float radius);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	virtual void TakeDown(); 
	//virtual void BuildNormals();
	virtual bool Update();

	float width;

	vec3 arbVec;
	std::vector<VertexP> baseSpline;
	std::vector<VertexP> childSpline;
	std::vector<VertexPFF> vertices;
	typedef Spline super;
};


