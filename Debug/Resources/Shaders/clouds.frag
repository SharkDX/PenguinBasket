#version 330 core

uniform sampler2D cloudTexture;
uniform float alphaValue = 1.0f;
in float light;
in vec2 UV;
out vec4 color;

void main()
{
	color = texture(cloudTexture, UV);
	color.rgb *= light;
	color.a *= alphaValue;
}
