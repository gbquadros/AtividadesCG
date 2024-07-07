#version 460

in vec4 finalColor;
in vec2 texCoord;
out vec4 color;

uniform sampler2D texBuff;
uniform sampler2D colorBuffer;

void main()
{
    vec4 texColor = texture(colorBuffer,texCoord);
    vec3 result = vec3(texColor);
    color = vec4(result, 1.0f);
 }