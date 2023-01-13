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

uniform vec2 direction;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 emissionColor;
layout(location = 3) out vec4 maskColor;
layout(location = 4) out vec4 posColor;
layout(location = 5) out vec4 uvColor;

uniform float blurMat[9] = float[](0.0162162162, 0.0540540541, 0.1216216216, 0.1945945946, 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

vec3 blur(vec2 dir, sampler2D tex, float w, float h)
{
    vec3 blur = vec3(0.0,0.0,0.0);
    for(int i = -4; i<5; i++)
    {
        blur += texture(tex, UV + vec2(i*dir.x/w, i*dir.y/h)).rgb * blurMat[i+4];
    }
    return blur;
}

void main()
{ 
    vec3 color = blur(direction, emissionTexture, width, height);
    
    fragmentColor = vec4(texture(screenTexture, UV).xyz, 1.0);
    normalColor = vec4(texture(normalTexture, UV).xyz, 1.0);
    emissionColor = vec4(color, 1.0);
    maskColor = vec4(texture(maskTexture, UV).xyz, 1.0);
    posColor = vec4(texture(posTexture, UV).xyz, 1.0);
    uvColor = vec4(texture(uvTexture, UV).xyz, 1.0);
}
