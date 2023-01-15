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

uniform float intensity;
uniform float delta;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

vec2 THRESHOLD = (0.0312, 0.125);

float Luma(vec3 rgb)//luminance in gamma
{
    return sqrt(0.299*rgb.x + 0.587*rgb.y + 0.144*rgb.z);
}

void main()
{ 
	vec3 fragColor;
    vec3 color = texture(screenTexture, UV).xyz;

	float center = Luma(color);
	float up    = texture(screenTexture, UV + vec2(0, 1.0f/height)).xyz;
	float down  = texture(screenTexture, UV + vec2(0, -1.0f/height)).xyz;
	float left  = texture(screenTexture, UV + vec2(-1.0f/width, 0)).xyz;
	float right = texture(screenTexture, UV + vec2(1.0f/width, 0)).xyz;

	float minLuma = min(center,min(min(down,up),min(left,right)));
	float maxLuma = max(center,max(max(down,up),max(left,right)));

	float lumaDelta = maxLuma - minLuma;

	if(lumaDelta < max(THRESHOLD.x, maxLuma*THRESHOLD.y))
	{
		fragColor = color;
	}

    fragmentColor = vec4(fragColor, 1.0);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
