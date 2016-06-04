#version 150
//view공간에 있음

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform bool in_lightVectorAsPosition;
uniform vec3 in_surfaceNormal, in_tangentVector, in_bitangentVector;
attribute vec2 in_texCoord;
attribute vec3 in_vertexPosition;

attribute vec3 in_vertexNormal;
attribute vec3 in_vertexHeight;

varying vec2 fragTexCoord;
varying vec3 TlightPos;
varying vec3 TcamPos;

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);
//http://sunandblackcat.com/tipFullView.php?topicid=28
void main()
{
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec4 viewPosition = modelViewMatrix * vec4(in_vertexPosition, 1);
	vec3 vertexViewPosition = transformPoint3(modelViewMatrix, in_vertexPosition);

	mat3 viewNormalMatrix = mat3(transpose(inverse(modelViewMatrix)));
	
	vec3 viewNormal = normalize(viewNormalMatrix * in_vertexNormal);
	vec3 viewTangent = normalize(viewNormalMatrix * in_tangentVector);

	vec3 fragLightVector = normalize(transformPoint3(in_viewMatrix, in_lightVector) - vertexViewPosition);

	vec3 viewDirToLight = normalize(fragLightVector - viewPosition.xyz);
	vec3 viewDirToCamera = normalize(-normalize(vertexViewPosition) - viewPosition.xyz); 

	vec3 viewBitangent = normalize(mat3(modelViewMatrix) * in_bitangentVector);//cross(viewNormal, viewTangent) * in_tangentVector.z;

	TlightPos = vec3(
		dot(viewDirToLight, viewTangent),
		dot(viewDirToLight, viewBitangent),
		dot(viewDirToLight, viewNormal)
	);
	TcamPos = vec3(
		dot(viewDirToCamera, viewTangent),
		dot(viewDirToCamera, viewBitangent),
		dot(viewDirToCamera, viewNormal)
	);
	fragTexCoord = in_texCoord;


	gl_Position = transformPoint4(in_projectionMatrix, vertexViewPosition);

	
}
	