#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 texture_coordinate;

uniform mat4 mvp;
out vec3 tc;

void main()
{
	tc = vertex_position;
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
