#version 330 core

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;

attribute vec3 in_vertexPosition;
attribute vec3 in_vertexNormal;
attribute vec2 in_texcoord;

vec4 transform(mat4 transformMatrix, vec3 vertexPosition);

out vec4 v2f_positionW;
out vec4 v2f_normalW;
out vec3 v2f_texcoord;

void main() {
	gl_Position = transform(in_projectionMatrix * in_viewMatrix * in_modelMatrix, in_vertexPosition);
	v2f_positionW = in_modelMatrix * vec4(in_position, 1);
	v2f_normalW = in_modelMatrix * vec4(in_vertexNormal, 0);
	v2f_texcoord = in_texcoord;
}