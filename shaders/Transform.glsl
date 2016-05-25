#version 110

vec4 transform(mat4 transformMatrix, vec3 vertexPosition) {
	vec4 pos = vec4(vertexPosition, 1);
	return transformMatrix * pos;
}