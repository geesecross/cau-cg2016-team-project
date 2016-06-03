#version 110
uniform sampler2D in_tex0;
uniform sampler2D in_normal;

uniform vec4 in_color;
uniform float in_ambientRatio, in_diffusionRatio, in_specularRatio, in_shiness;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;
varying vec3 fragPos;

varying vec3 TlightPos, TcamPos, TfragPos;


vec4 simpleIlluminationModel(vec4 faceColor, vec3 eyeVector, vec3 lightVector, vec3 normalVector, float ambientRatio, float diffusionRatio, float specularRatio, float shiness);
void main()
{
	vec3 normal = texture2D(in_normal, fragTexCoord).xyz;

	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture2D(in_tex0, fragTexCoord).xyz;

	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(TlightPos - TfragPos);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(TcamPos - TfragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//light, viewdir zero
	vec3 halfwayDir = normalize(fragLightVector + fragEyeVector);


	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	
	//specular is zero
	vec3 specular = vec3(0.2) * spec ;

	gl_FragColor = vec4(ambient + diffuse + specular, 0.0);


}