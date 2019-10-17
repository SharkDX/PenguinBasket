#version 330 core

uniform sampler2D blockTexture;

in float light;
in vec2 UV;
out vec4 color;

void main()
{
	color = texture(blockTexture, UV);
	color.rgb *= light;
}
