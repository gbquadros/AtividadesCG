#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texc;
layout (location = 2) in vec3 normal;

out vec3 finalColor;
out vec3 fragPos;
out vec2 texCoord;
out vec3 scaledNormal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	gl_Position = projection * view  * model * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0));
	texCoord = vec2(texc.x, 1-texc.y);
	scaledNormal = normal;
}