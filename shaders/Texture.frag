#version 110

// for illumination model
uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;
varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;
vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness);

// for diffuse texture
uniform sampler2D in_texDiffuse;
uniform bool in_texDiffuseExists;

void main() {
	vec4 color = vec4(1.0);
	if(in_texDiffuseExists) {
		color = texture2D(in_texDiffuse, fragTexCoord);
	}
	gl_FragColor = color * simpleIlluminationModel(in_color, fragEyeVector, fragLightVector, fragNormalVector, in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness);
}
