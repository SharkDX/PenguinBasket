#version 330 core

uniform mat4 MVP;
uniform vec2 pos;
uniform vec2 size;
uniform vec4 texRectx;
uniform vec4 texRecty;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 UV;

void main()
{
    gl_Position =  MVP * vec4(pos + position * size / vec2(32, 32), 0.0, 1.0f);
	
	if(texRectx.x == -1)
		UV = texcoord;
	else
		UV = vec2(texRectx[int(texcoord.x + texcoord.y * 2)], texRecty[int(texcoord.x + texcoord.y * 2)]);
}