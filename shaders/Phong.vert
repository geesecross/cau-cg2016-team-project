#version 110

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform bool in_lightVectorAsPosition;

attribute vec3 in_vertexPosition;
attribute vec3 in_vertexNormal;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;

vec4 transform(mat4 transformMatrix, vec3 vertexPosition);

void main() {
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec4 vertexViewPosition = transform(modelViewMatrix, in_vertexPosition);
	vec3 modelOriginPosition = transform(modelViewMatrix, vec3(0, 0, 0)).xyz;

	fragNormalVector = transform(modelViewMatrix, in_vertexNormal).xyz - modelOriginPosition;
	if(in_lightVectorAsPosition) {
		fragLightVector = normalize(transform(in_viewMatrix, in_lightVector).xyz - vertexViewPosition.xyz);
	}
	else {
		fragLightVector = normalize(transform(in_viewMatrix, in_lightVector).xyz - transform(in_viewMatrix, vec3(0, 0, 0)).xyz);
	}
	fragEyeVector = -normalize(vertexViewPosition.xyz);

	gl_Position = in_projectionMatrix * vertexViewPosition;
}