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

float intensity = 0.1;
float offset = 0.015;
vec2 RandomVector(vec2 uv, float offset)
{
	mat2 m = mat2(15.27,47.63,99.41,89.98);
	uv = fract(sin(mul(uv,m)) * 46839.32);
	return vec2(sin(uv.y+offset)*0.5+0.5, cos(uv.x+offset*0.5+0.5));
}

vec2 Voronoi(vec2 uv, float angle, float density)
{
	vec2 g = floor(uv * density);
	vec2 f = fract(uv * density);
	float t = 8.0;
	vec3 res = vec3(8.0, 0.0,0.0);

	for(int x=-1;x<2;x++)
	{
		for(int y=-1;y<2;y++)
		{
			vec2 lattice = vec2(x,y);
			vec2 offset = RandomVector(lattice+g, angle);
			float d = length(lattice+offset - f);
			if(d < res.x)
			{
				res = vec3(d, offset.x, offset.y);
			}
		}
	}

	return vec2(res.x, res.y);
}

float Luminance(vec3 rgb)
{
    return (0.299*rgb.x + 0.587*rgb.y + 0.144*rgb.z);
}

float stipple(float lum, vec2 voronoi)
{
	return (voronoi.x*intensity-offset > lum) ? voronoi.x : 1;
}

void main()
{
    vec4 albedoColor = vec4(texture(screenTexture, UV).xyz, 1.0);
	float density = 250;
	vec2 voronoi = Voronoi(UV, 15,  250);
	float lum= Luminance(albedoColor.rgb);
	lum = lum*lum;
	float vor = voronoi.y*voronoi.y;
	float stipple = (vor*intensity-offset<lum) ? 1 : stipple(lum, voronoi);
	albedoColor = vec4(stipple*albedoColor.rgb,1);
	//albedoColor = vec4(test,1);//vec4(vec3(stipple),1);
    fragmentColor = albedoColor;
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}