/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for EndlessRibbon class
*/
#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <queue>
#include "vertex.h" 
#define _USE_MATH_DEFINES 
#include <math.h>
#include "ribbon.h"
#include "endlessSpline.h"


class EndlessRibbon : public EndlessSpline
{
public:
	EndlessRibbon();
	virtual bool Initialize(glm::vec3 startPt, glm::vec3 endPt, glm::vec3 ctrlPtOne, glm::vec3 ctrlPtTwo, unsigned int lerps, int numSplines, glm::vec3 genDirection, float radius);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	virtual void TakeDown();
	virtual void UpdateRibbonDeque();
	
	int numSegs;
	float width;
	int modCounter;
	std::deque<Ribbon*> ribbonDeque;
	typedef EndlessSpline super;
};


