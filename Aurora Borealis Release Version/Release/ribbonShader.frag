#version 400

layout (location = 0) out vec4 FragColor;

uniform float time;

varying vec3 position;
varying float alpha_multi;

const vec3 color_red = vec3(1.0, 0.0, 0.0);
const vec3 color_green = vec3(0.0, 1.0, 0.0);
const vec3 color_blue = vec3(0.0, 0.0, 1.0);
const vec3 base_color = vec3(0.2, 0.6, 0.2);

void main()
{
	float modelX = position.x;
	float modelY = position.y;
	float modelZ = position.z;
	
	float redCoef = sin((time + modelY + modelZ) / 10);
	if(redCoef < 0)
		redCoef = -redCoef;

	float greenCoef = sin((time + modelY + modelZ) / 12);
	if(greenCoef < 0)
		greenCoef = -greenCoef;

	float blueCoef = cos((time + modelY + modelZ) / 14);
	if(blueCoef < 0)
		blueCoef = -blueCoef;

	float alphaOffset =  0.2 * sin(time + modelZ * 2) + 0.2 * cos(time / 2);
	float alphaSpread = 1.4 + 0.2 * sin(time + modelZ * 3);
	float alphaInput = modelY - 7.0f;
	float power = pow(alphaInput - alphaOffset, 2)/ (2 * pow(alphaSpread, 2));
	float alphaVal = (1 / alphaSpread) * 0.39894 * pow(2.71828, -power);
	float alphaMulti = alpha_multi + 0.3 * abs(sin(time + cos(modelZ * 3))) + 0.3 * cos(time + modelZ * 2);
	float alphaFade = (1 / (60.0 * 2.50663)) * pow(2.71828, -(pow((modelX - 120 - time), 2) / (2 * pow(60, 2)))) * 200.0;
	if(alphaFade > 1.0)
		alphaFade = 1.0;
	FragColor = vec4(redCoef * color_red + greenCoef * color_green + blueCoef * color_blue, alphaVal * alphaMulti * alphaFade * 1.5);
}