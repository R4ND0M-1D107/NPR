#version 330
out vec4 fragmentColor;
out vec4 normalColor;
out vec4 emissionColor;
out vec4 maskColor;
out vec4 posColor;
out vec4 uvColor;

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
	posColor = vec4(positionWS, 1.0);
	uvColor = vec4(tiledUV, 0.0, 1.0);
}