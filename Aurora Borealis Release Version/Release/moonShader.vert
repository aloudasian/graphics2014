#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec2 texture_coordinate;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

out vec2 tc;
varying vec3 position;
varying vec3 normal;

void main()
{
	tc = texture_coordinate;
	position = vertex_position;
	normal = normalize(normal_matrix * vertex_normal);
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
