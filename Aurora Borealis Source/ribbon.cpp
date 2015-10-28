/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Constructs and draws a Ribbon object
*/
#include "ribbon.h"

using namespace std;
using namespace glm;

Ribbon::Ribbon() : Spline()
{

}

//Clears and Takes down the Ribbon
void Ribbon::TakeDown()
{
	this->vertices.clear();
	this->childSpline.clear();
	this->baseSpline.clear();
	super::TakeDown();
}


bool Ribbon::Initialize(vec3 startPt, vec3 endPt, vec3 ctrlPtOne, vec3 ctrlPtTwo, unsigned int lerps, float radius)
{
	if (this->GLReturnedError("Ribbon::Initialize - on entry"))
		return false;

	//Checks if given parameters are valid for creation of basis Spline
	if (!super::Initialize(startPt, endPt, ctrlPtOne, ctrlPtTwo, lerps))
		return false;

	//Stores the vertices of the basis Spline
	for(unsigned int i = 0; i < super::vertices.size(); i++)
	{
		this->baseSpline.push_back(super::vertices[i]);
	}
	this->width = radius;
	
	//Calculates and stores the first vertex of the child Spline

	//cout<<"----------------------"<<endl;
	//cout<<"In Ribbon construction"<<endl;
	//cout<<" "<<endl;

	//create the child spline
	//cout<<"In child spline construction"<<endl;
	this->arbVec = normalize(vec3(0.0f, 0.0f, -1.0f));
	vec3 inSlope = normalize(this->ctrlPointOne.position - this->startPoint.position);
	if(inSlope.x == 0 && inSlope.y == 0 && inSlope.z != 0)
	{
		this->arbVec = normalize(vec3(0.001f, 0.001f, -1.0f));
		//cout<<"arbVec alteration executed"<<endl;
	}
	vec3 splineNormalVec = normalize(cross(inSlope, arbVec));
	this->childSpline.push_back(VertexP(splineNormalVec * this->width + this->startPoint.position));
	arbVec = normalize(vec3(0.0f, 0.0f, -1.0f));

	//Calcualtes and stores consecutive vertices of the child Spline 
	for(unsigned int i = 1; i < this->baseSpline.size() - 1; i++)
	{
		vec3 slope = normalize(this->baseSpline.at(i + 1).position - this->baseSpline.at(i - 1).position);
		if(slope.x == 0 && slope.y == 0 && slope.z != 0)
		{
			arbVec = normalize(vec3(0.001f, 0.001f, -1.0f));
			//cout<<"arbVec alteration executed"<<endl;
		}
		splineNormalVec = normalize(cross(slope, arbVec));
		this->childSpline.push_back(VertexP(splineNormalVec * this->width + this->baseSpline.at(i).position));
		arbVec = normalize(vec3(0.0f, 0.0f, -1.0f));
	}

	//Calculates and stores the last vertex of the child Spline
	vec3 outSlope = normalize(this->endPoint.position - this->ctrlPointTwo.position);
	if(outSlope.x == 0 && outSlope.y == 0 && outSlope.z != 0)
	{
		arbVec = normalize(vec3(0.001f, 0.001f, -1.0f));
		//cout<<"arbVec alteration executed"<<endl;
	}
	splineNormalVec = normalize(cross(outSlope, arbVec));
	this->childSpline.push_back(VertexP(splineNormalVec * this->width + this->endPoint.position));
	arbVec = normalize(vec3(0.0f, 0.0f, -1.0f));
	//cout<<"child spline construction success"<<endl;
	//cout<<"baseSpline size: "<<this->baseSpline.size()<<"   childSpline size: "<<this->childSpline.size()<<endl;

	//combines the base and child Spline vertices
	if(this->baseSpline.size() != childSpline.size())
	{
		cout<<"Dimension mismatch between base and child splines"<<endl;
		return false;
	}

	this->vertices.clear();
	for(unsigned int i = 0; i < this->baseSpline.size(); i++)
	{
		float intensity = (2.0f + fmod(this->baseSpline.at(i).position.x, this->baseSpline.at(i).position.z) / this->baseSpline.at(i).position.z) / 3.0f;
		this->vertices.push_back(VertexPFF(this->baseSpline.at(i).position, 2.0f, intensity));
		this->vertices.push_back(VertexPFF(this->childSpline.at(i).position, 2.0f, intensity));
	}
	//cout<<"ribbon vertices consturction success, vertices vector size is "<<this->vertices.size()<<endl;
	//cout<<"ribbon construction success"<<endl;
	//cout<<"----------------------"<<endl;

	//stores the order of vertices to form the Ribbon
	this->vertex_indices.clear();
	for(unsigned int i = 0; i < this->vertices.size() - 2; i += 2)
	{
		this->vertex_indices.push_back(i);
		this->vertex_indices.push_back(i+3);
		this->vertex_indices.push_back(i+1);

		this->vertex_indices.push_back(i);
		this->vertex_indices.push_back(i+2);
		this->vertex_indices.push_back(i+3);
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexPFF), &this->vertices[0]))
		return false;

	//re-stuffs buffers
	if(!Update())
	{
		cout<<"updating ribbon information failure"<<endl;
	}
	
	if (this->GLReturnedError("RibbionSegment::Initialize - on exit"))
		return false;

	return true;
}


//Draws the Ribbon segment
void Ribbon::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("RibbonSegment::Draw - on entry"))
		return;

	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//translates and moves the Ribbon as time passes
	modelview = translate(modelview, vec3(-100.0f, 0.0f, 0.0f));
	modelview = translate(modelview, vec3(-1.0f, 0.0f, 0.0f) * time);

	this->GLReturnedError("RibbonSegment::Draw - matrix use");
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));
	this->GLReturnedError("RibbonSegment::Draw - before use");
	this->ribbonShader.Use();
	this->GLReturnedError("RibbonSegment::Draw - after shader use");
	this->ribbonShader.Setup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	this->GLReturnedError("RibbonSegment::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES, this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("RibbonSegment::Draw - after draw");
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);

	if (this->GLReturnedError("RibbonSegment::Draw - on exit"))
		return;
}

bool Ribbon::Update()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPFF), (GLvoid *) 0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(VertexPFF), (GLvoid *) (sizeof(vec3) * 1));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(VertexPFF), (GLvoid *) (sizeof(float) + sizeof(vec3)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}
