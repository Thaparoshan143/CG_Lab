#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform mat4 proj;

out vec3 ourColor;

void main()
{
	gl_Position = proj * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	ourColor = aCol;
}