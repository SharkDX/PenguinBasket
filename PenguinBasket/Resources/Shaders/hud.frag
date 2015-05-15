#version 330 core

uniform sampler2D sampleTexture;
uniform vec3 tint;

in vec2 UV;
out vec4 color;

void main()
{
	color = texture(sampleTexture, UV);
	color.rgb += tint;
}
