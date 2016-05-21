#version 110

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 vertexPosition;

void main() {
	vec4 pos = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
}