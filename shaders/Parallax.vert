#version 150
uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform vec3 in_cameraPos;

attribute vec2 in_texCoord;
attribute vec3 in_vertexPosition;

attribute vec3 in_vertexNormal;
attribute vec3 in_vertexHeight;
//vec4 precomuted tangent

varying vec2 fragTexCoord;
varying vec3 TcamPos, TfragPos;
varying vec3 TlightPos;

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);
//http://sunandblackcat.com/tipFullView.php?topicid=28
void main()
{
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec3 vertexViewPosition = transformPoint3(modelViewMatrix, in_vertexPosition).xyz;
	vec4 worldPos = in_modelMatrix * vec4(in_vertexPosition, 1);

	//should be precomputed
	mat3 normalMatrix = mat3(transpose(inverse(in_viewMatrix * in_modelMatrix)));
	
	//vertex tangent calculation. should be precomputed
	vec3 c1 = cross(in_vertexNormal, vec3(0.0, 0.0, 1.0));
	vec3 c2 = cross(in_vertexNormal, vec3(0.0, 1.0, 0.0));
	vec3 vertexTangent;
	if(length(c1) > length(c2)){
		vertexTangent = c1;

	}
	else
		vertexTangent= c2;
	vertexTangent = normalize(vertexTangent);


	vec3 worldNormal = normalize(normalMatrix * in_vertexNormal);
	vec3 worldTangent = normalize(normalMatrix * vertexTangent);

	vec3 worldDirToLight = normalize(in_lightVector - worldPos.xyz);
	vec3 worldDirToCam = normalize(in_cameraPos - worldPos.xyz);

	vec3 worldBitangent = cross(worldNormal, worldTangent) * vertexTangent;
	TlightPos = vec3(
		dot(worldDirToLight, worldTangent),
		dot(worldDirToLight, worldBitangent),
		dot(worldDirToLight, worldNormal)
	);
	TcamPos = vec3(
		dot(worldDirToCam, worldTangent),
		dot(worldDirToCam, worldBitangent),
		dot(worldDirToCam, worldNormal)
	);
	fragTexCoord = in_texCoord;


	gl_Position = transformPoint4(in_projectionMatrix, vertexViewPosition);
}