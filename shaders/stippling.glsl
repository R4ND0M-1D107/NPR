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
uniform float scale;

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

float ContrastedLuminance(vec2 uv)
{
	float luminanceSurround = Luminance(texture(screenTexture, uv + vec2(-1/width, -1/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2(-1/width,  0/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2(-1/width,  1/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2( 0/width, -1/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2( 0/width,  1/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2( 1/width, -1/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2( 1/width,  0/height)).rgb)
	+ Luminance(texture(screenTexture, uv + vec2( 1/width,  1/height)).rgb);
	float luminance = Luminance(texture(screenTexture, uv).rgb);
	luminance = mix(luminance, luminanceSurround, intensity);
	return luminance;
}

float Rand(vec2 uv)
{      
	return 0.5+(fract(sin(dot(uv ,vec2(12.9898,78.233))) * 43758.5453))*0.5;
}

float DecolaratedNoise(vec2 uv, float size)
{
	float noise = Rand( floor(uv/size)*size);
	vec2 noise1 = uv + (noise);
	float blue = fract( noise1.x*1.7 + noise1.y/(1.85) );
	return blue;
}

void main()
{
    float noise = DecolaratedNoise(vec2(UV.x*width, UV.y*height) * scale, 2.0f);
	float luminance = ContrastedLuminance(UV);
	luminance = clamp(luminance, 0, 1);
	float stippling = step(noise, smoothstep(0.3, 0.8, luminance));
	
    fragmentColor = vec4(texture(screenTexture, UV).xyz*stippling, 1.0);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
