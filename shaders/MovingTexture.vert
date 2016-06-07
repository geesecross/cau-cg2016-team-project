#version 150

uniform mat4 in_modelMatrix, in_viewMatrix, in_projectionMatrix;
uniform vec3 in_lightVector;
uniform bool in_lightVectorAsPosition;


attribute vec2 in_texCoord;
attribute vec3 in_vertexPosition;
attribute vec3 in_vertexNormal;

varying vec3 fragNormalVector;
varying vec3 fragEyeVector;
varying vec3 fragLightVector;
varying vec2 fragTexCoord;

uniform vec3 in_surfaceNormal, in_tangentVector, in_bitangentVector;
out mat4 tangentMatrix;

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);

void main() {
	mat4 modelViewMatrix = in_viewMatrix * in_modelMatrix;
	vec3 vertexViewPosition = transformPoint3(modelViewMatrix, in_vertexPosition).xyz;
	
	tangentMatrix = transpose(mat4(
		vec4(in_tangentVector, 0), vec4(in_bitangentVector, 0), vec4(in_surfaceNormal, 0), vec4(0, 0, 0, 1.0)
	));

	fragNormalVector = transformDirection3(modelViewMatrix, in_vertexNormal);
	if(in_lightVectorAsPosition) {
		fragLightVector = normalize(transformPoint3(in_viewMatrix, in_lightVector) - vertexViewPosition);
	}
	else {
		fragLightVector = normalize(transformDirection3(in_viewMatrix, in_lightVector));
	}
	fragEyeVector = -normalize(vertexViewPosition);
	fragTexCoord = in_texCoord;
	//vertexViewPosition.x += cos(in_time)*0.1;
	//vertexViewPosition.y += cos(in_time)*0.1;
	gl_Position = transformPoint4(in_projectionMatrix, vertexViewPosition);


	fragLightVector = normalize(transformPoint3(in_viewMatrix, in_lightVector) - vertexViewPosition);
	fragNormalVector = transformDirection3(modelViewMatrix, in_vertexNormal);
	gl_Position = transformPoint4(in_projectionMatrix, vertexViewPosition);
}