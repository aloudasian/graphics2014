/* 
	UW- Madison CS 559 - Fall Semester
	By Zhouyang Michael Ye

	Creates and draws a Skybox object, code adopted from original authored by Perry Kivolowitz
*/
#include "skybox.h"

using namespace std;
using namespace glm;

Skybox::Skybox() : MeshBase()
{
	//Sets up necessary varibles
	this->cubemap_handle = BAD_GL_VALUE;
	this->number_of_textures = 6;
	this->texture_names.push_back(string("nightsky_south.tga"));
	this->texture_names.push_back(string("nightsky_up.tga"));
	this->texture_names.push_back(string("nightsky_north.tga"));
	this->texture_names.push_back(string("nightsky_west.tga"));
	this->texture_names.push_back(string("nightsky_east.tga"));
	this->texture_names.push_back(string("nightsky_down.tga"));
}

//Deletes the textures used in the Skybox and takes down the Skybox
void Skybox::TakeDown()
{
	if (this->cubemap_handle != BAD_GL_VALUE)
		glDeleteTextures(1, &this->cubemap_handle);

	this->vertices.clear();
	this->texture_names.clear();
	this->shader.TakeDown();
	super::TakeDown();
}

bool Skybox::Initialize()
{
	if (this->GLReturnedError("Skybox::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	mat4 m(1.0f);
	for (int i = 0; i < 4; i++)
	{
		m = rotate(mat4(1.0f), 90.0f * i, vec3(0.0f, 1.0f, 0.0f));
		m = translate(m, vec3(0.0f, 0.0f, 1.0f));
		this->PushVertices(m);
	}

	m = rotate(mat4(1.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
	m = translate(m, vec3(0.0f, 0.0f, 1.0f));
	this->PushVertices(m);

	m = rotate(mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f));
	m = translate(m, vec3(0.0f, 0.0f, 1.0f));
	this->PushVertices(m);

	glGenVertexArrays(1, &this->vertex_array_handle);
	glBindVertexArray(this->vertex_array_handle);
	glGenBuffers(1, &this->vertex_coordinate_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexPT), &this->vertices[0], GL_STATIC_DRAW);

	if (GLReturnedError("Skybox::PostGLInitialize - after geometry creation"))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid *) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid *) (sizeof(vec3) * 1));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (GLReturnedError("Skybox::PostGLInitialize - after attribute creation"))
		return false;

	if (!this->shader.Initialize("skybox.vert", "skybox.frag"))
		return false;

	if (GLReturnedError("Skybox::PostGLInitialize - after container initialize"))
		return false;

	//Loads and initializes textures used for the skybox
	int i = 0;
	for (vector<string>::iterator iter = this->texture_names.begin(); iter < this->texture_names.end(); iter++ , i++)
	{
		if (this->skybox_containers[i].Initialize(iter->c_str()) == false)
		{
			cerr << "Failed to load sky box texture: " << *iter << endl;
			return false;
		}
	}

	// http://www.keithlantz.net/2011/10/rendering-a-skybox-using-a-cube-map-with-opengl-and-glsl/

	if (GLReturnedError("Skybox::PostGLInitialize - after container initialize"))
		return false;

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &this->cubemap_handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap_handle);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (GLReturnedError("Skybox::PostGLInitialize - after cubemap texture creation."))
		return false;

	GLuint targets[6] = {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y};

	//Sets up the textures used in the skybox
	for (i = 0; i < this->number_of_textures; i++)
	{
		glTexImage2D(targets[i], 0, GL_RGBA, this->skybox_containers[i].width, this->skybox_containers[i].height, 0, this->skybox_containers[i].format, GL_UNSIGNED_BYTE, this->skybox_containers[i].data);
	}

	if (GLReturnedError("Skybox::PostGLInitialize - after cubemap texture storage assignment."))
		return false;

	return true;
}

//Stores the vertices of the Skybox
void Skybox::PushVertices(mat4 & m)
{
	this->vertices.push_back(VertexPT(vec3(m * vec4(-100.0f, -100.0f, 0.0f, 100.0f)), vec2(0.0f, 0.0f)));
	this->vertices.push_back(VertexPT(vec3(m * vec4(-100.0f,  100.0f, 0.0f, 100.0f)), vec2(1.0f, 0.0f)));
	this->vertices.push_back(VertexPT(vec3(m * vec4( 100.0f,  100.0f, 0.0f, 100.0f)), vec2(1.0f, 1.0f)));
	this->vertices.push_back(VertexPT(vec3(m * vec4( 100.0f, -100.0f, 0.0f, 100.0f)), vec2(0.0f, 1.0f)));
	unsigned int s = this->vertices.size() - 1;
	this->vertex_indices.push_back(s - 3);
	this->vertex_indices.push_back(s - 2);
	this->vertex_indices.push_back(s - 1);
	this->vertex_indices.push_back(s - 3);
	this->vertex_indices.push_back(s - 1);
	this->vertex_indices.push_back(s - 0);
}

//Draws the skybox
void Skybox::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	GLReturnedError("Skybox::Draw - on entry");
	mat4 mvp = projection * modelview;
	this->shader.Use();
	this->shader.CommonSetup(0.0f, value_ptr(size), NULL, NULL, value_ptr(mvp), NULL, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap_handle);
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES, this->vertex_indices.size(), GL_UNSIGNED_INT, &this->vertex_indices[0]);
	glBindVertexArray(0);
	glUseProgram(0);

	GLReturnedError("Skybox::Draw - on exit");
}

 