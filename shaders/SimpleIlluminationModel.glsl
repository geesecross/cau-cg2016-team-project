#version 110

vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness) {
	vec3 l = normalize(lightVector);
	vec3 v = normalize(eyeVector);
	vec3 n = normalize(normalVector);
	vec3 r = dot(n, l) > 0.0 ? normalize((2.0 * dot(n, l) * n) - l) : vec3(0, 0, 0);
	//vec3 h = dot(n, l) > 0.0 ? normalize(l + v) : vec3(0, 0, 0);

	//vec3 r = (2.0 * dot(n, l) * n) - l;

	float ambient = 1.0;
	//float diffusion = max(0.0, dot(n, l));
	float diffusion = max(-1.0, dot(n, l));
	float specular = pow(max(0.0, dot(v, r)), shiness);
	//float specular = pow(max(0.0, dot(n, h)), shiness);

	if(diffusion < 0.0) {
		specular = 0.0;
		if(diffusionRatio > 0.0)
			diffusion *= ambientRatio / diffusionRatio;
		else
			diffusion = 0.0;
	}

	return vec4(faceColor.xyz * (ambientRatio * ambient + diffusionRatio * diffusion + specularRatio * specular), faceColor.w);
}