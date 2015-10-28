/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Header file for Vertex classes
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class VertexPN
{
public:
	VertexPN();
	VertexPN(const vec3 & pos, const vec3 & norm);
	VertexPN(const VertexPN & otherVertex);
	vec3 position;
	vec3 normal;
};

class VertexP
{
public:
	VertexP();
	VertexP(const vec3 & pos);
	VertexP(const VertexP & otherVertex);
	vec3 position;
};

class VertexPCN
{
public:
	VertexPCN();
	VertexPCN(const glm::vec3 & p, const glm::vec3 & c, const glm::vec3 & n);
	VertexPCN(const VertexPCN & other);
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};

class VertexPC
{
public:
	VertexPC();
	VertexPC(const glm::vec3 & p, const glm::vec3 & c);
	VertexPC(const VertexPC & other);
	glm::vec3 position;
	glm::vec3 color;
};

class VertexPCNT
{
public:
	VertexPCNT();
	VertexPCNT(const glm::vec3 & p, const glm::vec3 & c, const glm::vec3 & n, const glm::vec2 & t);
	VertexPCNT(const VertexPCNT & other);
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texture_coordinate;
};

class VertexPT
{
public:
	VertexPT();
	VertexPT(const glm::vec3 & p, const glm::vec2 & t);
	VertexPT(const VertexPT & other);
	glm::vec3 position;
	glm::vec2 texture_coordinate; 
};

class VertexPFF
{
public:
	VertexPFF();
	VertexPFF(const glm::vec3 & p, const float & alphaOffset, const float & alphaIntensity);
	VertexPFF(const VertexPFF & other);
	glm::vec3 position;
	float alphaOffset;
	float alphaIntensity;
};