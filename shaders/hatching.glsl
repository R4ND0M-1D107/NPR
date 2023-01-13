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

struct Light
{
	vec3 color;
	vec3 distribution; //ambient diffuse specular
	vec4 position;
	vec3 direction;
	float cutoff;
	float exponent;
};

uniform Light lights[4];
uniform vec3 viewerPosition;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

vec3 SampleNormalTexture(sampler2D normalTex, vec2 UV)
{
    vec3 normal = texture(normalTex, UV).xyz;
    vec3 decodedNormal = normal * 2.0f - 1.0f;
    return decodedNormal;
}

vec2 hatchingDirection(vec2 screenUV)
{
	vec2 texOffset = 1.0 / textureSize(uvTexture, 0);
	vec2 localUV1 = texture(uvTexture, UV).xy;
	vec2 localUV2 = texture(uvTexture, UV + texOffset).xy;
	vec2 ret = normalize(localUV1-localUV2);
	return ret;
}

float hatch(float uvx)
{
	//float val = 1-clamp(sin(uvx*3000), 0, 1);
	float val = abs(sin(uvx*300)); 
	//floor(fract(uvx*20) + 0.5);
	return val;
}

vec4 computeLight(Light light, vec3 n, vec3 positionWS, vec4 mask)
{
	vec3 color = vec3(0.0f);

	if(light.position.w == 0.0) //directional
	{
		vec3 l = normalize(light.direction.xyz);
		n = normalize(n);
		float cosAlpha = max(0, dot(l, n));
		vec3 r = reflect(-l, n);
		vec3 v = normalize(viewerPosition - positionWS);
		float cosBeta = max(0, dot(r, v));
		float shininess = mask.a;

		color += light.color * light.distribution.r * mask.g; //ambient
		color += light.color * light.distribution.b * cosAlpha; //diffuse
	} else if(light.position.w == 1.0)
	{
		vec3 l = normalize(light.position.xyz - positionWS);
		n = normalize(n);
		float cosAlpha = max(0, dot(l, n));
		vec3 r = reflect(-l, n);
		vec3 v = normalize(viewerPosition - positionWS);
		float cosBeta = max(0, dot(r, v));
		float shininess = mask.a;

		float spotCos = max(dot(-l, normalize(light.direction)), 0.0);
		if(light.direction == vec3(0.0f,0.0f,0.0f) || spotCos >= light.cutoff)
		{
			color += light.color * light.distribution.r * mask.g; //ambient
			color += light.color * light.distribution.g * cosAlpha; //diffuse
			color += light.color * light.distribution.b * pow(cosBeta, shininess)  * mask.rrr; //specualar
		}

		if(light.direction == vec3(0.0,0.0,0.0))
		{
			float distance = length(light.position.xyz-positionWS);
			float power = 1.0f / (1.0f + 0.5f*distance + 0.01*distance*distance);
			color *= power;
		} else
		{
			pow(spotCos*color, vec3(light.exponent));
		}
	}

	color = clamp(color, vec3(0,0,0), vec3(1,1,1));
	return vec4(color, 0.0);
}

void main()
{
    vec4 albedoColor = vec4(0.0, 0.0, 0.0, 1.0); 

    vec3 normal = SampleNormalTexture(normalTexture, UV);
	vec4 mask = texture(maskTexture, UV);
	vec3 pos = texture(posTexture, UV).xyz;
	vec4 fragColor = texture(screenTexture, UV);

	for(int i = 0; i<4; i++)
	{
		albedoColor += computeLight(lights[i], normal, pos, mask);
	}
	albedoColor.a = texture(screenTexture, UV).a;


	float hatching = 1;
	float borders[] = {0.35, 0.2, 0.1};
	for(int i = 0; i<3; i++)
	{
		if(albedoColor.r > borders[i]) break;
		vec2 hatchDir = hatchingDirection(UV);
		vec2 hatchUV = vec2(hatchDir.x*UV.x, hatchDir.y*UV.y);
		float uvx = mix(hatchDir.x, hatchDir.y, 0.5+i/2.0);
		hatching = hatching * hatch(uvx);
		//hatching -= 0.33f;
	}

	//fragmentColor = vec4(hatchingDirection(UV), 0, 1);
    fragmentColor = albedoColor;//texture(screenTexture, UV);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
