#version 150

uniform sampler2D in_tex0;
uniform sampler2D in_normal;
uniform sampler2D in_height;

uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;
uniform float parallaxScale;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;
varying vec3 fragPos;


varying vec3 TcamPos, TfragPos;
varying vec3 TlightPos;
out vec4 resultingColor;


vec2 parallaxMapping(in vec3 V, in vec2 T, out float parallaxHeight){
	/*const float minLayer = 5;
	const float maxLayer = 15;

	float numLayers= mix(maxLayer, minLayer, abs(dot(vec3(0, 0, 1), V)));

	float layerHeight = 1.0 / numLayers;
	float currentLayerHeight = 0;

	vec2 dtex = parallaxScale * V.xy / V.z / numLayers;

	vec2 currentTexCoords = T;
	
	float textureHeight = texture(in_height, currentTexCoords).r;

	while(textureHeight > currentLayerHeight){
		currentLayerHeight += layerHeight;
		currentTexCoords -= dtex;
		textureHeight = texture(in_height, currentTexCoords).r;

	}

	parallaxHeight = currentLayerHeight;
	return currentTexCoords;*/

	float initHeight = texture(in_height, fragTexCoord).r;
	vec2 texCoordOffset = parallaxScale * V.xy / V.z * initHeight;
	texCoordOffset = parallaxScale * V.xy * initHeight;
	return fragTexCoord - texCoordOffset;

}
float parallaxSoftShadowMult(vec3 L, vec2 initTexCoord, float initHeight){
	return 1.0;
}

vec4 normalMapLighting(vec2 T, vec3 L, vec3 V, float shadowMult){
	vec3 N = normalize(texture(in_normal, T).xyz * 2 - 1);
	vec3 D = texture(in_tex0, T).rgb;

	//ambient
	float iamb = 0.2;
	//diffuse
	float idiff = clamp(dot(N, L), 0, 1);

	//spec
	float ispec;
	if(dot(N, L) > 0.2){
		vec3 R = reflect(-L, N);
		ispec = pow(dot(R, V), 21);
	}

	vec4 resColor;
	resColor.rgb = D * (iamb + (idiff + ispec) * pow(shadowMult, 4));
	resColor.a = 1;
	return resColor;

}
void main(){
	vec3 V = normalize(TcamPos);
	vec3 L = normalize(TlightPos);

	float parallaxHeight;
	vec2 T = parallaxMapping(V, fragTexCoord, parallaxHeight);

	float shadowMult = parallaxSoftShadowMult(L, T, parallaxHeight - 0.05);

	gl_FragColor = normalMapLighting(T,L,V, shadowMult);
//	resultingColor = normalMapLighting(T,L,V, shadowMult);
}