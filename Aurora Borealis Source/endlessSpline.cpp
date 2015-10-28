/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Stores and maintains a deque of Spline objects, creates and removes Spline objects as time passes
*/
#include <iostream>
#include "endlessSpline.h"

using namespace std;
using namespace glm;

EndlessSpline::EndlessSpline() : Spline()
{

}

//Generates a pseudo-random, positive float ranging from 0 to 1
float genRandPosFloat()
{
	return (float)(rand() / (float)RAND_MAX);
}

//Generates a pseudo-random float, ranging from -1 to 1
float genRandFloat()
{
	return genRandPosFloat() - genRandPosFloat();
}

//Generates a pseudo-random float base on input, ranging from -f to f
float genRandVal(float f)
{
	return genRandFloat() * f;
}

//Destroys the EndlessSpline object
void EndlessSpline::TakeDown()
{
	//Takes down each Spline pointer in the deque
	for(unsigned int i = 0; i < this->splineDeque.size(); i++)
	{
		this->splineDeque.at(i)->TakeDown();
	}

	this->splineDeque.clear();
	super::TakeDown();
}

bool EndlessSpline::Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps, int numSplines, vec3 genDirection)
{
	if (this->GLReturnedError("EndlessSpline::Initialize - on entry"))
		return false;

	//Checks if given parameters are valid for the initial Spline
	if (!super::Initialize(startPt, endPt, ctrlPtOne, ctrlPtTwo, lerps))
		return false;

	//cout<<"----------------------"<<endl;
	//cout<<"In EndlessSpline construction"<<endl;
	//cout<<" "<<endl;
	//initialize variables
	this->numSplines = numSplines;
	this->genDirection = normalize(genDirection);

	//Initializes and stores the first Spline

	//cout<<"Entering subSpline 0 construction"<<endl;
	Spline * startSpline = new Spline;
	startSpline->Initialize(startPt, endPt, ctrlPtOne, ctrlPtTwo, lerps);
	//cout<<"Return from subSpline 0 construction"<<endl;
	//cout<<" "<<endl;
	this->splineDeque.push_back(startSpline);

	//Initializes and stores consecutive Spline objects, maintaining C1 connectivity
	//construct and insert subsequent splines
	for(this->segmentCounter = 1; this->segmentCounter < this->numSplines; this->segmentCounter++)
	{
		//cout<<"In EndlessSpline subSpline "<<this->segmentCounter<<" setup"<<endl;
		//cout<<" "<<endl;
		//generates values for the start point and first control point of the next Spline based on values of the previous Spline
		vec3 startPt = this->splineDeque.back()->endPoint.position;
		vec3 ctrlPtOne = startPt + (this->splineDeque.back()->endPoint.position - this->splineDeque.back()->ctrlPointTwo.position);
		//generates pseudo-random values for the end point and second control point of next Spline
		vec3 endPt = this->genDirection * 3.0f * (float)(this->segmentCounter + 1) + vec3(genRandFloat(), 0.0f, genRandFloat());
		vec3 ctrlPtTwo = this->genDirection * 3.0f * ((float)this->segmentCounter + 0.5f) + vec3(0.0f, 0.0f, genRandFloat() * 5.0f);

		//cout<<"subSpline start point at "<<startPt.x<<" "<<startPt.y<<" "<<startPt.z<<endl;
		//cout<<"subSpline end point at "<<endPt.x<<" "<<endPt.y<<" "<<endPt.z<<endl;
		//cout<<"subSpline control point one at "<<ctrlPtOne.x<<" "<<ctrlPtOne.y<<" "<<ctrlPtOne.z<<endl;
		//cout<<"subSpline control point two at "<<ctrlPtTwo.x<<" "<<ctrlPtTwo.y<<" "<<ctrlPtTwo.z<<endl;
		//cout<<"subSpline lerps = "<<lerps<<endl;
		//cout<<" "<<endl;

		//Creates and stores the new Spline object
		Spline * s = new Spline;
		//cout<<"entering subSpline "<<this->segmentCounter<<" construction"<<endl;
		s->Initialize(startPt, endPt, ctrlPtOne, ctrlPtTwo, lerps);
		//cout<<"return from subSpline construction"<<endl;
		//cout<<" "<<endl;
		this->splineDeque.push_back(s);
		
	}
	//cout<<"Exit EndlessSpline construction"<<endl;
	//cout<<"----------------------"<<endl;

	return true;
}

//Draws each Spline stored in the deque
void EndlessSpline::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	for(unsigned int i = 0; i < this->splineDeque.size(); i++)
	{
		this->splineDeque.at(i)->Draw(projection, modelview, size, time);
	}
}

//Updates the deque of Splines when called, removes the Spline object at the front of the deque, creates new Spline object and stores to
//the back of the deque, maintaining C1 connectivity similar to EndlessSpline Initialization
void EndlessSpline::UpdateSplineDeque()
{
	this->splineDeque.pop_front();
	//cout<<"In EndlessSpline subSpline NEW subspline setup"<<endl;
	//cout<<" "<<endl;
	int i = splineDeque.size();
	vec3 startPt = this->splineDeque.back()->endPoint.position;
	vec3 ctrlPtOne = startPt + (this->splineDeque.back()->endPoint.position - this->splineDeque.back()->ctrlPointTwo.position);
	vec3 endPt = this->genDirection * 3.0f * (float)(this->segmentCounter + 1) + vec3(genRandFloat(), 0.0f, genRandFloat());
	vec3 ctrlPtTwo = this->genDirection * 3.0f * ((float)this->segmentCounter + 0.5f) + vec3(0.0f, 0.0f, genRandFloat() * 5.0f /* ctrlPtOne.z*/);

	//cout<<"subSpline start point at "<<startPt.x<<" "<<startPt.y<<" "<<startPt.z<<endl;
	//cout<<"subSpline end point at "<<endPt.x<<" "<<endPt.y<<" "<<endPt.z<<endl;
	//cout<<"subSpline control point one at "<<ctrlPtOne.x<<" "<<ctrlPtOne.y<<" "<<ctrlPtOne.z<<endl;
	//cout<<"subSpline control point two at "<<ctrlPtTwo.x<<" "<<ctrlPtTwo.y<<" "<<ctrlPtTwo.z<<endl;
	//cout<<"subSpline lerps = "<<lerps<<endl;
	//cout<<" "<<endl;

	Spline * s = new Spline;
	//cout<<"entering subSpline NEW subspline construction"<<endl;
	s->Initialize(startPt, endPt, ctrlPtOne, ctrlPtTwo, (unsigned int)this->lerps);
	//cout<<"return from NEW subSpline construction"<<endl;
	//cout<<" "<<endl;
	this->splineDeque.push_back(s);
	//cout<<"Exit NEW subspline construction"<<endl;
	//cout<<"----------------------"<<endl;
	this->segmentCounter++;
} 

