#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in float alphaOffset;
layout (location = 2) in float alphaIntensity;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform float time;

varying vec3 position;
varying float alpha_multi;

void main()
{
	alpha_multi = alphaIntensity;
	position = vertex_position;
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
