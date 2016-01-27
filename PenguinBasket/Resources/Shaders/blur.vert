#version 330 core

uniform mat4 Projection;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 UV;

void main()
{
	vec2 pos = position * vec2(1280, 720);
    gl_Position =  Projection * vec4(pos.x, 720 - pos.y, 0.0f, 1.0f);
	UV = texcoord;
}