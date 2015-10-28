#version 400

layout (location = 0) out vec4 FragColor;

in vec2 tc;
uniform sampler2D mySampler;

void main()
{
	FragColor = texture(mySampler, tc);
}
