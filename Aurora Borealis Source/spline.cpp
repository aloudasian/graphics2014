/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Creates and draws a Bezier Spline object based on given parameters
*/
#include <iostream>
#include "spline.h"

using namespace std;
using namespace glm;

Spline::Spline() : MeshBase()
{

}

void Spline::TakeDown()
{
	this->vertices.clear();
	super::TakeDown();
}


bool Spline::Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps)
{
	if (this->GLReturnedError("Spline::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	//cout<<"----------------------"<<endl;
	//cout<<"In Spline Construction"<<endl;
	//cout<<" "<<endl;
	//cout<<"start point: "<<startPt.x<<" "<<startPt.y<<" "<<startPt.z<<endl;
	//cout<<"end point: "<<endPt.x<<" "<<endPt.y<<" "<<endPt.z<<endl;
	//cout<<"control point one: "<<ctrlPtOne.x<<" "<<ctrlPtOne.y<<" "<<ctrlPtOne.z<<endl;
	//cout<<"control point two: "<<ctrlPtTwo.x<<" "<<ctrlPtTwo.y<<" "<<ctrlPtTwo.z<<endl;
	//cout<<"number of lerps: "<<lerps<<endl;
	//cout<<" "<<endl;

	//Stores information needed for De-Casteljau Algorithm
	this->startPoint = VertexP(startPt);
	this->endPoint = VertexP(endPt);
	this->ctrlPointOne = VertexP(ctrlPtOne);
	this->ctrlPointTwo = VertexP(ctrlPtTwo);
	this->lerps = (float)lerps;
	this->t = (float)(1.0f / (float)this->lerps);

	//Calculates consecutive vertices in the Spline using the De-Casteljau Algorithm
	vertices.push_back(this->startPoint);
	for(float i = t; i < 1.0f; i += this->t)
	{
		vec3 ab = Lerp(this->startPoint.position, this->ctrlPointOne.position, i);
		vec3 bc = Lerp(this->ctrlPointOne.position, this->ctrlPointTwo.position, i);
		vec3 cd = Lerp(this->ctrlPointTwo.position, this->endPoint.position, i);
		vec3 abbc = Lerp(ab, bc, i);
		vec3 bccd = Lerp(bc, cd, i);
		vec3 dest = Lerp(abbc, bccd, i);
		vertices.push_back(VertexP(dest));
	}
	this->vertices.push_back(this->endPoint);

	//cout<<"Spline vertices construction success, spline has "<<vertices.size()<<" vertices"<<endl;
	//cout<<"Exit Spline construction"<<endl;
	//cout<<"----------------------"<<endl;

	//Assigns linear vertex association 
	for(unsigned int i = 0; i < vertices.size() - 1; i++)
	{
		this->vertex_indices.push_back(i);
		this->vertex_indices.push_back(i + 1);
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexP), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), (GLvoid *) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

//Utility method for calculating linear interpolations
vec3 Spline::Lerp(vec3 & start, vec3 & end, float & t)
{
	float xPos = start.x + (end.x - start.x) * t;
	float yPos = start.y + (end.y - start.y) * t;
	float zPos = start.z + (end.z - start.z) * t;
	return vec3(xPos, yPos, zPos);
}

//Draws the Spline object
void Spline::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("SplineSegment::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	this->GLReturnedError("SplineSegment::Draw - matrix use");
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));
	this->GLReturnedError("SplineSegment::Draw - before use");
	this->splineShader.Use();
	this->GLReturnedError("SplineSegment::Draw - after shader use");
	this->splineShader.Setup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	this->GLReturnedError("SplineSegment::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_LINES, this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("SplineSegment::Draw - after draw");
	glUseProgram(0);

	if (this->GLReturnedError("SplineSegment::Draw - on exit"))
		return;
}

 