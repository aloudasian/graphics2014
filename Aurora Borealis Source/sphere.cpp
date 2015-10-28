/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Creates and draws a Sphere mesh of given dimensions capable of being drawn with a texture
*/
#include <iostream>
#include "sphere.h"
#define _USE_MATH_DEFINES 
#include <math.h>

using namespace std;
using namespace glm;

//Default constructor, default color set to white
Sphere::Sphere() : MeshBase()
{

}

bool Sphere::Initialize(float radius, vec2 dimensions, char * vertexShader, char* fragShader)
{
	
	if (this->GLReturnedError("Sphere::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (radius < 0)
		radius = 1;

	//sets up values necessary for sphere mesh creation
	this->radius = radius;
	float yDegree = 90.0f;
	float xzDegree = 0.0f;
	float yStep = -180.0f / dimensions.y;
	float xStep = 360.0f / dimensions.x;

	cout<<this->vertices.size()<<endl;
	//creates and stores vertices of the mesh, also calculates UV coordinates for each vertex
	for(int stack = 0; stack <= dimensions.y; stack++)
	{
		for(int slice = 0; slice <= dimensions.x; slice++)
		{
			float xPos = cos(toRadian(xzDegree)) * cos(toRadian(yDegree));
			float yPos = sin(toRadian(yDegree));
			float zPos = sin(toRadian(xzDegree)) * cos(toRadian(yDegree));

			vec3 position = vec3(xPos, yPos, zPos) * radius;
			
			vec2 UV = vec2(1.0f / dimensions.x * slice, 1.0f / dimensions.y * stack);
			this->vertices.push_back(VertexPCNT(position, vec3(0.0f, 0.0f, 0.0f), normalize(vec3(position)),UV));
			xzDegree = fmod(xzDegree + xStep, 360.0f);
			
		}
		yDegree = yDegree + yStep;
	}

	//assigns vertex index association to create the triangle mesh
	for(int stack = 0; stack <= dimensions.y; stack++)
	{
		for(int slice = 0; slice <= dimensions.x; slice++)
		{
			int vertexIndex = (int)(stack * dimensions.x + slice);

			this->vertex_indices.push_back(vertexIndex);
			this->vertex_indices.push_back(vertexIndex + (unsigned int)dimensions.x);
			this->vertex_indices.push_back(vertexIndex + (unsigned int)dimensions.x + 1);

			this->vertex_indices.push_back(vertexIndex);
			this->vertex_indices.push_back(vertexIndex + (unsigned int)dimensions.x + 1);
			this->vertex_indices.push_back(vertexIndex + 1);
		}
	}


	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexPCNT), &this->vertices[0]))
		return false;

	//sets up and stuffs VAOs and Buffers for vector of vertices and vector of indices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNT), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNT), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPCNT), (GLvoid *) (sizeof(vec3) * 3));	// Note offset - legacy of older code
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Initializes shaders based upon input
	if (!this->starShader.Initialize(vertexShader, fragShader))
		return false;

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

//clears any relevant vectors, VAOs, and buffers
void Sphere::TakeDown()
{
	this->vertices.clear();
	this->starShader.TakeDown();
	super::TakeDown();
}

//Draws the mesh with given projection and modelview matrices, window size, and time elapsed
void Sphere::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("SphereMesh::Draw - on entry"))
		return;
	
	//rotates the mesh as time passes
	modelview = rotate(modelview, time * 1.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = rotate(modelview, 180.0f, vec3(1.0f, 0.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);

	this->GLReturnedError("SphereMesh::Draw - matrix use");
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));
	this->GLReturnedError("SphereMesh::Draw - before use");
	this->starShader.Use();
	this->GLReturnedError("SphereMesh::Draw - after use");
	this->starShader.Setup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	this->GLReturnedError("SphereMesh::Draw - after common setup");
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture_handle);
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("SphereMesh::Draw - after draw");
	glUseProgram(0);

	if (this->GLReturnedError("SphereMesh::Draw - on exit"))
		return;
}

  