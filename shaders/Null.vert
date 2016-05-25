#version 110

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;

attribute vec3 in_vertexPosition;

vec4 transform(mat4 transformMatrix, vec3 vertexPosition);

void main() {
	gl_Position = transform(in_projectionMatrix * in_viewMatrix * in_modelMatrix, in_vertexPosition);
}