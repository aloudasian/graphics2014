#version 400

layout (location = 0) out vec4 FragColor;

in vec3 tc;
uniform samplerCube cubemap;

void main()
{
	vec3 tc2 = vec3(tc.x, tc.y, tc.z);
	FragColor = texture(cubemap, tc2);
}
