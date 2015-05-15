#version 330 core

uniform sampler2D animationTexture;
uniform float light = 1;

in vec2 UV;
out vec4 color;

void main()
{
	color = texture(animationTexture, UV);
	color.rgb *= light;
}
