#version 330 core

uniform mat4 MVP;
uniform vec2 pos;
uniform float type;
uniform vec2 size;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 UV;
out float light;

void main()
{
    gl_Position =  MVP * vec4(pos + position * size, 0.0, 1.0f);
	UV = vec2(texcoord.x, type / 2 + texcoord.y / 2);
	light = 1.0f;
}