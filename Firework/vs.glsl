#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 MVP;

out vec4 color;

void main()
{
	gl_Position = MVP * aPos;
	color = aColor;
} 