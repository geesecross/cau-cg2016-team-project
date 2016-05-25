#version 110

uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;

vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness);

void main() {
	gl_FragColor = simpleIlluminationModel(in_color, fragEyeVector, fragLightVector, fragNormalVector, in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness);
}