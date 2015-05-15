#version 330 core

uniform sampler2D blockTexture;

in float light;
in vec2 UV;
out vec4 color;

void main()
{
	/*vec4 texColor = vec4(0, 0, 0, 0);;
	for(float i = -sampleCount; i <= sampleCount; i++) {
		for(float j = -sampleCount; j <= sampleCount; j++) {
			texColor += texture(blockTexture, UV + vec2(i / 1280.0, j / 720.0));
		}
	}
	texColor /= pow(sampleCount * 2 + 1, 2);
	*/
	color = texture(blockTexture, UV);
	color.rgb *= light;
}
