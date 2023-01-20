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
	samplerCube shadowCubemap;
	sampler2D shadowMap;
	mat4 lightSpaceMat;
};

uniform Light lights[3];
uniform vec3 viewerPosition;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

vec2 dirs[4] = vec2[4]
(
	vec2( 5.0f/width, -5.0f/height),
	vec2(-5.0f/width,  5.0f/height),
	vec2(-5.0f/width, -5.0f/height),
	vec2( 5.0f/width,  5.0f/height)
);

float ShadowCalculation(vec3 fragPos, vec3 lightPos, samplerCube shadowCubemap)
{
	//softer shadows
	//float shadow = 0;
	//for(int i = 0; i<4; i++)
	//{
	//	fragPos = texture(posTexture, UV+dirs[i]).xyz  * 25.0f;
	//	vec3 fragToLight = fragPos - lightPos;
	//	float closestDepth = texture(shadowCubemap, fragToLight).r;
	//	closestDepth *= 25.0f;
	//	float currentDepth = length(fragToLight);
	//	float bias = 0.05; 
	//	shadow += currentDepth -  bias > closestDepth ? 0.0 : 1.0;
	//}
    //shadow *= 0.25;

	//hard shadow
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(shadowCubemap, fragToLight).r;
	closestDepth *= 25.0f;
	float currentDepth = length(fragToLight);
	float bias = 0.05; 
	float shadow = currentDepth -  bias > closestDepth ? 0.0 : 1.0;

    return shadow;
}  

float ShadowCalculation(vec3 fragPos, mat4 mat, vec3 lightPos, sampler2D shadowMap)
{
	vec4 fragPosLS = mat*vec4(fragPos,1);
	vec3 coords = fragPosLS.xyz/fragPosLS.w;
	coords = coords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, coords.xy).r;
	float currentDepth = coords.z;
	float bias = 0.05; 
	float shadow = currentDepth -  bias > closestDepth ? 0.0 : 1.0;
	return 1;
}

vec3 SampleNormalTexture(sampler2D normalTex, vec2 UV)
{
    vec3 normal = texture(normalTex, UV).xyz;
    vec3 decodedNormal = normal * 2.0f - 1.0f;
    return decodedNormal;
}

vec4 computeLight(Light light, vec3 n, vec3 positionWS, vec4 mask, vec3 fragColor)
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

		float shadow = ShadowCalculation(positionWS, light.lightSpaceMat, light.position.xyz, light.shadowMap);

		color += light.color * light.distribution.r * fragColor * mask.g; //ambient
		color += light.color * light.distribution.b * cosAlpha * fragColor * shadow; //diffuse
		color += light.color * light.distribution.b * pow(cosBeta, shininess*shininess)  * mask.r * shadow; //specualar
	} else if(light.position.w == 1.0)
	{
		vec3 l = normalize(light.position.xyz - positionWS);
		n = normalize(n);
		float cosAlpha = max(0, dot(n, l));
		vec3 r = reflect(-l, n);
		vec3 v = normalize(viewerPosition - positionWS);
		float cosBeta = max(0, dot(r, v));
		float shininess = mask.a;

		float spotCos = max(dot(-l, normalize(light.direction)), 0.0);
		if(light.direction == vec3(0.0,0.0,0.0) || spotCos >= light.cutoff)
		{
			float shadow = ShadowCalculation(positionWS, light.position.rgb, light.shadowCubemap);
			color += light.color * light.distribution.r * fragColor * mask.g; //ambient
			color += light.color * light.distribution.g * cosAlpha * fragColor * shadow; //diffuse
			color += light.color * light.distribution.b * pow(cosBeta, shininess*shininess)  * mask.r *shadow; //specualar
		}

		if(light.direction == vec3(0.0,0.0,0.0))
		{
			float distance = length(light.position.xyz-positionWS);
			float power = 1.0f / (1.0f + 0.5f*distance + 0.01*distance*distance);
			color *= power;
		} else
		{
			color = pow(spotCos*color, vec3(light.exponent));
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
	vec3 pos = texture(posTexture, UV).xyz * 50.0f - 25.0f;
	vec4 fragColor = texture(screenTexture, UV);

	for(int i = 0; i<3; i++)
	{
		albedoColor += computeLight(lights[i], normal, pos, mask, fragColor.rgb);
	}
	albedoColor.a = texture(screenTexture, UV).a;

    fragmentColor = albedoColor;
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
