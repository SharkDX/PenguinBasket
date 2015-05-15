#version 330 core

uniform sampler2D textureMap;
uniform vec2 blurType = vec2(1, 0);
const float weight[5] = float[5]( 0.16, 0.15, 0.12, 0.09, 0.05 );
const vec2 blurSize = vec2(1.0/1280.0, 1.0/720.0);

in vec2 UV;
out vec4 color;

void main()
{
	vec4 total = vec4(0,0,0,0);
	
	for(int i = 0; i < 5; i++) {
		total += texture(textureMap, UV + blurType * blurSize * i, 2.0) * weight[i];
		if(i != 0)
			total += texture(textureMap, UV - blurType * blurSize * i, 2.0) * weight[i];
	}
	
	total.a = total.r * total.a > 0.3 ? 1.0 : 0.0;
	color = total;
}
