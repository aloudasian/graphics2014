#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec2 texture_coordinate;

uniform mat4 mvp;
out vec2 tc;

void main()
{
	tc = texture_coordinate;
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
