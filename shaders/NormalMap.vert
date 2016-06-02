#version 150
uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform vec3 in_cameraPos;
uniform bool in_lightVectorAsPosition;
uniform float in_time;

attribute vec2 in_texCoord;
attribute vec3 in_vertexPosition;
attribute vec3 in_vertexTangent;
attribute vec3 in_vertexNormal;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;
varying vec3 fragPos;

varying vec3 TlightPos, TcamPos, TfragPos;

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);
void main() {
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec3 vertexViewPosition = transformPoint3(modelViewMatrix, in_vertexPosition).xyz;

	vec3 vertexBitangent = cross(in_vertexTangent, in_vertexNormal);

	mat3 normalMatrix = transpose(inverse(mat3(in_modelMatrix)));

	vec3 T = normalize(normalMatrix * in_vertexTangent);
	vec3 B = normalize(normalMatrix  * vertexBitangent);
	vec3 N = normalize(normalMatrix * in_vertexNormal);

	mat3 TBN = transpose(mat3(T,B,N));
	
	fragPos = vec3(in_modelMatrix * vec4(in_vertexPosition, 1.0));

	TlightPos = TBN * in_lightVector;
	TcamPos = TBN * in_cameraPos;
	TfragPos = TBN * fragPos;


	fragNormalVector = transformDirection3(modelViewMatrix, in_vertexNormal);
	if(in_lightVectorAsPosition) {
		fragLightVector = normalize(transformPoint3(in_viewMatrix, in_lightVector) - vertexViewPosition);
	}
	else {
		fragLightVector = normalize(transformDirection3(in_viewMatrix, in_lightVector));
	}

	fragEyeVector = -normalize(vertexViewPosition);

	fragTexCoord = in_texCoord;



	gl_Position = transformPoint4(in_projectionMatrix, vertexViewPosition);
}
