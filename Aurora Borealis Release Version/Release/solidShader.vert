#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 UV;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
