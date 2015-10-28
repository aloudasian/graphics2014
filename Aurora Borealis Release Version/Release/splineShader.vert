#version 400

layout (location = 0) in vec3 vertex_position;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

out vec3 position;

void main()
{
	position = vec3(modelview_matrix * vec4(vertex_position,1.0));
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
