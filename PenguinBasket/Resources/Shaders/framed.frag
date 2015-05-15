#version 330 core

uniform sampler2D frameTexture;
uniform float light = 1;
uniform float alphaValue = 1;

in vec2 UV;
out vec4 color;

void main()
{
	color = texture(frameTexture, UV);
	color.rgb *= light;
	color.a *= alphaValue;
}
