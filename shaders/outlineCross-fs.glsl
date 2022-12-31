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

out vec4 fragmentColor;
out vec4 normalColor;
out vec4 emissionColor;
out vec4 maskColor;
out vec4 posColor;
out vec4 uvColor;

vec3 SampleNormalTexture(sampler2D normalTex, vec2 UV)
{
    vec3 normal = texture(normalTex, UV).xyz;
    vec3 decodedNormal = normal * 2.0f - 1.0f;
    return decodedNormal;
}

float NormalOutline(sampler2D tex)
{
    vec3 horizontal      = SampleNormalTexture(tex, UV + vec2(-1.0f/width, -1.0f/height)*delta);
    horizontal          -= SampleNormalTexture(tex, UV + vec2( 1.0f/width,  1.0f/height)*delta);

    vec3 vertical        = SampleNormalTexture(tex, UV + vec2(-1.0f/width,  1.0f/height)*delta);
    vertical            -= SampleNormalTexture(tex, UV + vec2( 1.0f/width, -1.0f/height)*delta);

    float outline        = sqrt(dot(horizontal, horizontal) + dot(vertical, vertical));
    float outlineBias = (1/intensity) * sqrt(dot(SampleNormalTexture(tex, UV),SampleNormalTexture(tex, UV)));
    outline              = (outline < outlineBias) ? 1 : 1-clamp(outline,0.0,1.0);

    return outline;
}

void main()
{ 
    float outline = NormalOutline(normalTexture);
    vec3 color = texture(screenTexture, UV).xyz;


    fragmentColor = vec4(color*outline, 1.0);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
