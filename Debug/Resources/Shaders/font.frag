#version 330 core

uniform sampler2D tex;
uniform vec4 color = vec4(1, 1, 1, 1);

in vec2 texcoord;
out vec4 outcolor;
 
void main(void) {
  outcolor = vec4(color.rgb, color.a * texture(tex, texcoord).a);
}