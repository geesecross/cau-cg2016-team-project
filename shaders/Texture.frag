#version 330 core

in vec2 texcoord0;
varying vec4 fragColor;
uniform sampler2D tex0;

layout (location=0) out vec4 out_color;

void main() {
	out_color = texture(tex0, texcoord0);
}