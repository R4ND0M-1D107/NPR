#version 330
in vec2 UV;

uniform sampler2D screenTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D emissionTexture;
uniform sampler2D maskTexture;
uniform sampler2D posTexture;
uniform sampler2D uvTexture;

uniform float width;
uniform float height;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

float hatch(float uvx)
{
	//float val = 1-clamp(sin(uvx*3000), 0, 1);
	float val = abs(sin(uvx*300)); 
	val = clamp((val+0.35), 0, 1);
	//floor(fract(uvx*20) + 0.5);
	return val;
}

float Luminance(vec3 rgb)
{
    return (0.299*rgb.x + 0.587*rgb.y + 0.144*rgb.z);
}

void main()
{
    vec4 albedoColor = vec4(texture(screenTexture, UV).xyz, 1.0);

	float hatching = 1;
	float borders[4] = float[](0.1, 0.085, 0.05, 0.025);
	float mixVal[4] = float[](0.25, 0.5, 0.75, 1);
	for(int i = 0; i<4; i++)
	{
		float luminance = Luminance(albedoColor.rgb);
		if(luminance > borders[i]) break;
		vec2 hatchDir = texture(uvTexture, UV).xy * 0.5f;
		float uvx = mix(hatchDir.x, hatchDir.y, mixVal[i]);
		hatching = hatching * hatch(uvx);
	}
	
    fragmentColor = albedoColor*hatching;
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
