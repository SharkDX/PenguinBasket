#version 330 core

uniform sampler2D textureMap;
uniform sampler2D maskMap;
const vec2 pixelSize = vec2(1.0/1280.0, 1.0/720.0);

in vec2 UV;
out vec4 color;

void main()
{
	int counter = 0;
	vec4 total = vec4(0,0,0,0);
	color = texture(textureMap, UV);

	if (texture(maskMap, UV).a > 0) {
		for (int i = 0; i <= 1; i++) {
			for(int j = 0; j <= 1; j++) {
				total += texture(textureMap, UV - vec2(i, j) * pixelSize * 1.0f);
				++counter;
			}
		}
		total /= counter;
		color = texture(textureMap, UV);
		color.rgb /= 2.0f;
	}
}
