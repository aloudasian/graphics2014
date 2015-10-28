/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Stores and maintains a deque of Ribbon objects, creates and removes Ribbon objects as time passes
*/
#include "endlessRibbon.h"

using namespace std;
using namespace glm;

EndlessRibbon::EndlessRibbon() : EndlessSpline()
{
	
}

//Destroys the EndlessRibbon object
void EndlessRibbon::TakeDown()
{
	//Takes down each Ribbon pointer in the deque
	for(unsigned int i = 0; i < this->ribbonDeque.size(); i++)
	{
		this->ribbonDeque.at(i)->TakeDown();
	}

	this->ribbonDeque.clear();
	super::TakeDown();
}

bool EndlessRibbon::Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps, int numSplines, vec3 genDirection, float width)
{
	if (this->GLReturnedError("EndlessRibbon::Initialize - on entry"))
		return false;

	//Initializes a deque of Spline objects
	if (!super::Initialize(startPt, endPt, ctrlPtOne, ctrlPtTwo, lerps, numSplines, genDirection))
		return false;

	//cout<<"----------------------"<<endl;
	//cout<<"In EndlessRibbon construction"<<endl;
	//cout<<" "<<endl;

	this->numSegs = super::numSplines;
	this->width = width;
	this->modCounter = 1;
	
	//Constructs and stores consecutive ribbon based upon individual splines from inherited EndlessSpline
	for(int i = 0; i < numSplines; i++)
	{
		Ribbon * rib = new Ribbon;

		//cout<<"Entering subRibbon "<<i<<" construction"<<endl;
		rib->Initialize(this->splineDeque.at(i)->startPoint.position, this->splineDeque.at(i)->endPoint.position, this->splineDeque.at(i)->ctrlPointOne.position, this->splineDeque.at(i)->ctrlPointTwo.position, (unsigned int)this->splineDeque.at(i)->lerps, this->width);
		//cout<<"Return from subRibbon construction"<<endl;
		//cout<<" "<<endl;
		this->ribbonDeque.push_back(rib);
		//if(i != 0)
		//{
		//	FixEndNormals(i);
		//}
	}
	//cout<<"EndlessRibbon construction success"<<endl;
	//cout<<"----------------------"<<endl;

	return true;
}

//Draws each ribbon stored in the deque, updates the deque when necessary
void EndlessRibbon::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	//If sufficient time has passed, update the deque of Splines, then proceed to update the deque of Ribbons
	if(time > 3.0f * this->modCounter)
	{
		super::UpdateSplineDeque();
		UpdateRibbonDeque();
		this->modCounter++;
	}

	for(unsigned int i = 0; i < this->ribbonDeque.size(); i++)
	{
		this->ribbonDeque.at(i)->Draw(projection, modelview, size, time);
	}
}

//Removes the Ribbon stored at the front of the deque, create and store a new Ribbon based on a new Spline to the back of the deque
void EndlessRibbon::UpdateRibbonDeque()
{
	this->ribbonDeque.pop_front();
	Ribbon * rib = new Ribbon;
	//cout<<"Entering subRibbon NEW ribbon construction"<<endl;
	rib->Initialize(this->splineDeque.back()->startPoint.position, this->splineDeque.back()->endPoint.position, this->splineDeque.back()->ctrlPointOne.position, this->splineDeque.back()->ctrlPointTwo.position, (unsigned int)this->splineDeque.back()->lerps, this->width);
	//cout<<"Return from NEW subRibbon construction"<<endl;
	//cout<<" "<<endl;
	this->ribbonDeque.push_back(rib); 
	/*FixEndNormals(this->ribbonDeque.size() - 1);*/
}