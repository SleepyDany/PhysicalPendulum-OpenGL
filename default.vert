#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float scale;

void main()
{
	gl_Position = vec4(aPosition.x + aPosition.x * scale, aPosition.y + aPosition.y * scale, aPosition.z + aPosition.z * scale, 1.0);
	color = aColor;
}