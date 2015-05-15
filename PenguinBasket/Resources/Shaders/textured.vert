#version 330 core

uniform mat4 MVP;
uniform vec2 blockPos;
uniform float blockIndex = 1;
uniform float spriteIndex;
uniform vec4 lightMap;
uniform float blockRender = 1;
uniform vec2 size = vec2(1,1);

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 UV;
out float light;

void main()
{
    gl_Position =  MVP * vec4(blockPos + (blockRender != 1 ? position * size : position), 0.0, 1.0f);
	UV = blockRender != 1 ? texcoord : vec2((blockIndex * 3 + (int(spriteIndex) % 3) + texcoord.x) / 48.0, (int(spriteIndex / 3) + texcoord.y) / 3.0);
	light = lightMap[int((position.x / 32.0) + (position.y / 32.0) * 2)];
}