#version 330
in vec2 position;
in vec2 vertexUV;

out vec2 UV;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
    UV = vertexUV;
}
