#version 330 core

uniform sampler2D textureMap;
uniform vec2 blurType = vec2(0, 0);
const vec2 pixelSize = vec2(1.0/1280.0, 1.0/720.0);
uniform float blurSize = 1.0f;

in vec2 UV;
out vec4 color;

void main()
{

	vec4 total = vec4(0,0,0,0);
	
	/*for(int i = 0; i < 5; i++) {
		total += texture(textureMap, UV + blurType * blurSize * i, 2.0) * weight[i];
		if(i != 0)
			total += texture(textureMap, UV - blurType * pixelSize * blurSize * i, 2.0) * weight[i];
	}*/
	int counter = 0;
	for (int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			total += texture(textureMap, UV - vec2(i, j) * pixelSize * blurSize);
			++counter;
		}
	}
	total /= counter;
	
	if(blurSize > 0.0f)
	{
		total.a = total.r * total.a > 0.5 ? 1.0 : 0.0;
	}
	color = total;
}
