
#version 110

uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;
uniform sampler2D in_tex0;
uniform sampler2D in_texSpecular;
uniform float in_time;


varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;
vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness);

void main() {
vec4 texColor = texture2D(in_tex0, vec2(fragTexCoord.x + in_time, fragTexCoord.y + in_time));

gl_FragColor = simpleIlluminationModel(in_color, fragEyeVector, fragLightVector, fragNormalVector, in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness) * texColor;




	

}