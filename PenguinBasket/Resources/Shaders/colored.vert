#version 330 core

uniform mat4 MVP;
uniform vec2 size;
uniform vec2 pos;

layout(location = 0) in vec2 position;

out vec2 UV;

void main()
{
    gl_Position =  MVP * vec4(pos + position * size, 0.0f, 1.0f);
}