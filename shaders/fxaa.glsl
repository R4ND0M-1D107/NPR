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

float contrastThreshold = 0.0312
float relativeThreshold = 0.125;

float Luminance(vec3 rgb)//luminance in gamma
{
    return (0.299*rgb.x + 0.587*rgb.y + 0.144*rgb.z);
}

mat3 sampleLuminance(sampler2D tex, vec2 uv)
{
	mat3 samples;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			samples[i][j] = texture(tex, uv + vec2((i-1.0f)/width, (j-1.0f)/height)).xyz;
		}
	}
}

vec2 MinimaxCross(mat3 samples) //horizontal vertical, min in x, max in y
{
	vec2 minimax;
	minimax.x = min(samples[1][1],min(min(samples[1][0],samples[1][2]),min(samples[0][1],samples[2][1])));
	minimax.y = max(samples[1][1],max(min(samples[1][0],samples[1][2]),max(samples[0][1],samples[2][1])));
}

float BlendFactor(mat3 sample, float luminanceDelta)
{
	float blend = 2 * (samples[1][0] + samples[1][2] +samples[0][1] + samples[2][1]); //horizontal and vertical have double weight
	blend += samples[0][0] + samples[2][2] +samples[0][2] + samples[2][0]; //diagonal - technicaly further therefore less weight
	blend /= 12.0f; //divide by total weight to get average luminosity of surroundings
	blend = abs(blend- samples[1][1]); //contrast between current pixel and surroundings
	blend = saturate(filter/luminanceDelta); //relative contrast to cross contrast and clamp it to one
	blend = smoothstep(0,1 blend); //smooth out transition
	return  blend*blend; //slow it down
}

void main()
{ 
	vec3 fragColor;
    vec3 color = texture(screenTexture, UV).xyz;
	
	mat3 luminanceSample = sampleLuminance();
	vec2 luminanceMinimax = MinimaxCross(luminanceSample);
	float luminanceDelta = minimax.y - minimax.x;

	if(luminanceDelta < max(contrastThreshold, minimax.y*relativeThreshold))
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
