#version 110

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform bool in_lightVectorAsPosition;

attribute vec3 in_vertexPosition;
attribute vec3 in_vertexNormal;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);

void main() {
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec3 vertexViewPosition = transformPoint3(modelViewMatrix, in_vertexPosition).xyz;

	fragNormalVector = transformDirection3(modelViewMatrix, in_vertexNormal);
	if(in_lightVectorAsPosition) {
		fragLightVector = normalize(transformPoint3(in_viewMatrix, in_lightVector) - vertexViewPosition);
	}
	else {
		fragLightVector = normalize(transformDirection3(in_viewMatrix, in_lightVector));
	}
	fragEyeVector = -normalize(vertexViewPosition);

	gl_Position = transformPoint4(in_projectionMatrix, vertexViewPosition);
}
