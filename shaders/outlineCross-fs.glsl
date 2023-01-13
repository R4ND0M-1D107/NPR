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

float SampleDepthTexture(sampler2D tex, vec2 uv)
{
    return pow(texture(tex, uv).r, 50);
}

float DepthOutline(sampler2D tex)
{
    float horizontal      = SampleDepthTexture(tex, UV + vec2(-1.0f/width, -1.0f/height)*delta);
    horizontal          -= SampleDepthTexture(tex, UV + vec2( 1.0f/width,  1.0f/height)*delta);

    float vertical        = SampleDepthTexture(tex, UV + vec2(-1.0f/width,  1.0f/height)*delta);
    vertical            -= SampleDepthTexture(tex, UV + vec2( 1.0f/width, -1.0f/height)*delta);

    float outline        = sqrt(dot(horizontal, horizontal) + dot(vertical, vertical));
    float outlineBias = (1/intensity/50) * sqrt(dot(SampleDepthTexture(tex, UV),SampleDepthTexture(tex, UV)));
    outline              = (outline < outlineBias) ? 1 : 1-clamp(outline,0.0,1.0);

    return outline;
}

void main()
{ 
    float outline = min(DepthOutline(depthTexture), NormalOutline(normalTexture));
    vec3 color = texture(screenTexture, UV).xyz;


    fragmentColor = vec4(color*outline, 1.0);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(texture(emissionTexture, UV).xyz, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
