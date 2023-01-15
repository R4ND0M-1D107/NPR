#version 330
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

in vec2 UV;
in mat3 TBN;
in vec3 positionWS;

uniform mat4  projectionMatrix;   // transformation matrix
uniform mat4  viewMatrix;   // transformation matrix
uniform mat4  modelMatrix;   // transformation matrix

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D emissionTexture;
uniform sampler2D maskTexture;

//uniform vec2 tiling;

void main()
{
	vec2 tiledUV = UV;//vec2(mod(UV.x*tiling.x,1), mod(UV.y*tiling.y,1));
	vec3 normal = vec3(texture(normalTexture, tiledUV)) * 2.0 - 1.0;
	vec4 normalWS = vec4((TBN * normal), 1.0);

	normalColor = normalWS;
	fragmentColor = texture(albedoTexture, tiledUV);
	emissionColor = vec4(texture(emissionTexture, tiledUV));
	maskColor = texture(maskTexture, tiledUV);
	posColor = vec4(positionWS/25.0f, 1.0);
	uvColor = vec4(tiledUV, 0.0, 1.0);
}