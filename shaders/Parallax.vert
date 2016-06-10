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

varying vec3 fragEyeVector;
varying vec3 fragLightVector;

out mat4 tangentMatrix;

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);
//http://sunandblackcat.com/tipFullView.php?topicid=28
void main()
{
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec3 vertexViewPosition = transformPoint3(modelViewMatrix, in_vertexPosition);

	tangentMatrix = transpose(mat4(
		vec4(in_tangentVector, 0), vec4(in_bitangentVector, 0), vec4(in_surfaceNormal, 0), vec4(0, 0, 0, 1.0)
	));

	if(in_lightVectorAsPosition) {
		fragLightVector = normalize(transformPoint3(in_viewMatrix, in_lightVector) - vertexViewPosition);
	}
	else {
		fragLightVector = normalize(transformDirection3(in_viewMatrix, in_lightVector));
	}

	fragEyeVector = -normalize(vertexViewPosition);

	vec3 viewDirToLight = fragLightVector;
	vec3 viewDirToCamera = normalize(-vertexViewPosition); 
	
	vec3 viewNormal = transformDirection3(modelViewMatrix, in_surfaceNormal);
	vec3 viewTangent = transformDirection3(modelViewMatrix, in_tangentVector);
	vec3 viewBitangent = transformDirection3(modelViewMatrix, in_bitangentVector); //cross(viewNormal, viewTangent) * in_tangentVector.z;

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
	