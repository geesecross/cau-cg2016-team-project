#version 110

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform bool in_lightVectorAsPosition;
uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;

attribute vec3 in_vertexPosition;
attribute vec3 in_vertexNormal;

varying vec4 fragColor;

vec4 transform(mat4 transformMatrix, vec3 vertexPosition);
vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness);

void main() {
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec4 vertexViewPosition = transform(modelViewMatrix, in_vertexPosition.xyz);
	vec3 modelOriginPosition = transform(modelViewMatrix, vec3(0, 0, 0)).xyz;
	
	vec3 viewEyeVector = -vertexViewPosition.xyz;
	vec3 viewLightVector;
	if(in_lightVectorAsPosition) {
		viewLightVector = normalize(transform(in_viewMatrix, in_lightVector).xyz - vertexViewPosition.xyz);
	}
	else {
		viewLightVector = normalize(transform(in_viewMatrix, in_lightVector).xyz - transform(in_viewMatrix, vec3(0, 0, 0)).xyz);
	}
	vec3 viewNormalVector = transform(modelViewMatrix, in_vertexNormal).xyz - modelOriginPosition;

	fragColor = simpleIlluminationModel(in_color, viewEyeVector, viewLightVector, viewNormalVector, in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness);
	gl_Position = in_projectionMatrix * vertexViewPosition;
}