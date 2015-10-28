/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Constructs various Vertex Attribute Classes
*/
#include "vertex.h"

using namespace glm;

//Vertex consisting of position and normal
VertexPN::VertexPN()
{
	this->position = vec3(0.0f);
	this->normal = vec3(0.0f);
}

VertexPN::VertexPN(const vec3 & pos, const vec3 & norm)
{
	this->position = pos;
	this->normal - norm;
}

VertexPN::VertexPN(const VertexPN & otherVertex)
{
	this->position = otherVertex.position;
	this->normal = otherVertex.normal;
}

//Vertex consisting of only position
VertexP::VertexP()
{
	this->position = vec3(0.0f);
}

VertexP::VertexP(const vec3 & pos)
{
	this->position = pos;
}

VertexP::VertexP(const VertexP & otherVertex)
{
	this->position = otherVertex.position;
}

//Vertex consisting of position, normal, and color
VertexPCN::VertexPCN()
{
	this->position = vec3(0.0f);
	this->color = vec3(0.0f);
	this->normal = vec3(0.0f);
}

VertexPCN::VertexPCN(const vec3 & p, const vec3 & c, const vec3 & n)
{
	this->position = p;
	this->color = c;
	this->normal = n;
}

VertexPCN::VertexPCN(const VertexPCN & other)
{
	this->position = other.position;
	this->color = other.color;
	this->normal = other.normal;
}

//Vertex consisting of position and color
VertexPC::VertexPC()
{
	this->position = vec3(0.0f);
	this->color = vec3(0.0f);
}

VertexPC::VertexPC(const vec3 & p, const vec3 & c)
{
	this->position = p;
	this->color = c;
}

VertexPC::VertexPC(const VertexPC & other)
{
	this->position = other.position;
	this->color = other.color;
}

//Vertex consisting of position, normal, color, and texture coordinates
VertexPCNT::VertexPCNT()
{
	this->position = vec3(0.0f);
	this->color = vec3(0.0f);
	this->normal = vec3(0.0f);
	this->texture_coordinate = vec2(0.0f);
}

VertexPCNT::VertexPCNT(const vec3 & p, const vec3 & c, const vec3 & n, const vec2 & t)
{
	this->position = p;
	this->color = c;
	this->normal = n;
	this->texture_coordinate = t;
}

VertexPCNT::VertexPCNT(const VertexPCNT & other)
{
	this->position = other.position;
	this->color = other.color;
	this->normal = other.normal;
	this->texture_coordinate = other.texture_coordinate;
}

//Vertex consisting of position and texture coordinates
VertexPT::VertexPT()
{
	this->position = vec3(0.0f);
	this->texture_coordinate = vec2(0.0f);
}

VertexPT::VertexPT(const vec3 & p, const vec2 & t)
{
	this->position = p;
	this->texture_coordinate = t;
}

VertexPT::VertexPT(const VertexPT & other)
{
	this->position = other.position;
	this->texture_coordinate = other.texture_coordinate;
}

//Vertex consisting of position and two floats for alpha value calcuation
VertexPFF::VertexPFF()
{
	this->position = vec3(0.0f);
	this->alphaOffset = 0.0f;
	this->alphaIntensity = 1.0f;
}

VertexPFF::VertexPFF(const vec3 & p, const float & alphaOffset, const float & alphaIntensity)
{
	this->position = p;
	this->alphaOffset = alphaOffset;
	this->alphaIntensity = alphaIntensity;
}

VertexPFF::VertexPFF(const VertexPFF & other)
{
	this->position = other.position;
	this->alphaOffset = other.alphaOffset;
	this->alphaIntensity = other.alphaIntensity;
}

 