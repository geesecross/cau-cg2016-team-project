#version 110
uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform sampler2D in_tex0;
uniform sampler2D in_normal;

uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;

uniform vec3 in_surfaceNormal, in_tangentVector, in_bitangentVector;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;
varying vec3 fragPos;

in mat4 tangentMatrix;
varying vec3 TlightPos, TcamPos, TfragPos;

vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);
vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness);

void main() {
	vec3 mapNormal = texture2D(in_normal, fragTexCoord).xyz - vec3(0.5);
	mapNormal = transformDirection3(in_viewMatrix * in_modelMatrix * tangentMatrix, mapNormal);
	vec4 texColor = texture2D(in_tex0, fragTexCoord);

	
	gl_FragColor = texColor * simpleIlluminationModel(in_color, fragEyeVector, fragLightVector, mapNormal, in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness);
	//gl_FragColor = texture2D(in_normal, fragTexCoord);
}