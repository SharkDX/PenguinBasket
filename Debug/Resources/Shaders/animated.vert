#version 330 core

uniform mat4 MVP;
uniform float frame;
uniform float frameCount;
uniform float line;
uniform float lineCount;
uniform vec2 pos;
uniform vec2 size = vec2(1, 1);
uniform float dir = 1.0;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 UV;

void main()
{
	vec2 temp = position;
	if(dir == -1.0)
		temp.x = 32 - temp.x;
		
    gl_Position =  MVP * vec4(pos + temp * size, 0.0, 1.0f);
	UV = vec2((frame + texcoord.x) / frameCount, (line + texcoord.y) / lineCount);
}