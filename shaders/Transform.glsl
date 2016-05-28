#version 110

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point);
vec3 transformPoint3(in mat4 transformMatrix, in vec3 point);
vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector);

vec4 transformPoint4(in mat4 transformMatrix, in vec3 point) {
	return transformMatrix * vec4(point, 1.0);
}

vec3 transformPoint3(in mat4 transformMatrix, in vec3 point) {
	return (transformMatrix * vec4(point, 1.0)).xyz;
}

vec3 transformDirection3(in mat4 transformMatrix, in vec3 vector) {
	return (transformMatrix * vec4(vector, 0.0)).xyz;
}

vec4 transform(mat4 transformMatrix, vec3 vertexPosition) {
	vec4 pos = vec4(vertexPosition, 1);
	return transformMatrix * pos;
}
