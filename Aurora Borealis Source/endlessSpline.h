/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for EndlessSpline class
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
#include <deque>
#include "shader.h"
#include "meshBase.h"
#include "spline.h"

class EndlessSpline : public Spline
{
public:
	EndlessSpline();
	virtual bool Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps, int numSplines, glm::vec3 genDirection);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	virtual void TakeDown();
	virtual void UpdateSplineDeque();

	int numSplines;
	int segmentCounter;
	vec3 genDirection;
	std::deque<Spline*> splineDeque;
	typedef Spline super;
};