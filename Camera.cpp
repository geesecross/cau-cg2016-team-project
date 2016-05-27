#define _USE_MATH_DEFINES

#include "Matrix.h"
#include "MatrixFactory.h"
#include "FileHelper.h"
#include "Model.h"

#include "Camera.h"

#include <cmath>

Camera::Camera() {
	this->viewPlaneNormal = { 0, 0, 1 };
	this->viewUpVector = { 0, 1, 0 };
	this->viewport.width = 300;
	this->viewport.height = 300;
	this->setOrthographicProjection();
	this->updateViewMatrix();
}

Camera & Camera::setScale(GLfloat scale) {
	this->scale = scale;
	this->updateProjectionMatrix();
	return *this;
}

const Vector3f & Camera::getViewReferencePoint() const {
	return this->viewReferencePoint;
}

Camera & Camera::setViewReferencePoint(const Vector3f & point){
	this->viewReferencePoint = point;
	this->updateViewMatrix();
	return *this;
}

const Vector3f & Camera::getViewPlaneNormal() const {
	return this->viewPlaneNormal;
}

Camera & Camera::setViewPlaneNormal(const Vector3f & viewPlaneNormal) {
	this->viewPlaneNormal = viewPlaneNormal.normalized();
	this->viewUpVector = (this->viewUpVector - this->viewPlaneNormal * this->viewUpVector.dot(this->viewPlaneNormal)).normalized();
	this->updateViewMatrix();
	return *this;
}

Camera & Camera::move(const Vector3f & vector) {
	this->viewReferencePoint += this->viewUpVector * vector[1];
	this->viewReferencePoint -= this->viewPlaneNormal * vector[2];
	this->viewReferencePoint += this->viewUpVector.cross(this->viewPlaneNormal).normalized() * vector[0];
	this->updateViewMatrix();
	return *this;
}

Camera & Camera::rotate(const float radian) {
	Matrix4f mat = MatrixFactory::rotation(radian, MatrixFactory::Y);
	Vector4f n = mat * Vector4f(this->viewPlaneNormal, 1);
	this->viewPlaneNormal = Vector3f({ n[0], n[1], n[2] }).normalized();
	this->updateViewMatrix();

	return *this;
}

const Matrix4f& Camera::getViewMatrix() const {
	return this->viewMatrix;
}

void Camera::updateViewMatrix() {
	Vector3f v = this->viewUpVector.normalized();
	Vector3f n = this->viewPlaneNormal.normalized();
	Vector3f u = v.cross(n);

	Matrix4f translationMat = Matrix4f({
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-this->viewReferencePoint[0], -this->viewReferencePoint[1], -this->viewReferencePoint[2], 1
	});

	Matrix4f rotationMat = Matrix4f({
		u[0], v[0], n[0], 0,
		u[1], v[1], n[1], 0,
		u[2], v[2], n[2], 0,
		0, 0, 0, 1
	});

	// for OpenGL 1.0
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vector3f look = this->viewReferencePoint - this->viewPlaneNormal;
	gluLookAt(this->viewReferencePoint[0], this->viewReferencePoint[1], this->viewReferencePoint[2],
		look[0], look[1], look[2],
		this->viewUpVector[0], this->viewUpVector[1], this->viewUpVector[2]);
	//glGetFloatv(GL_MODELVIEW_MATRIX, this->viewMatrix.data());
	
	// for Vertex Shader
	this->viewMatrix = rotationMat * translationMat;
}

Camera & Camera::setViewMatrix(const Matrix4f& matrix) {
	this->viewMatrix = matrix;
	return *this;
}

const Matrix4f& Camera::getProjectionMatrix() const {
	return this->projectionMatrix;
}

Camera & Camera::setProjectionMatrix(const Matrix4f& matrix) {
	this->projectionMatrix = matrix;
	return *this;
}

Camera & Camera::setOrthographicProjection() {
	float aspectRatio = (float) this->getAspectRatio();
	float width = 2000, height = 2000, depth = 10000;

	// for OpenGL 1.0 commands
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2 * aspectRatio, width / 2 * aspectRatio, -height / 2, height / 2, 0, -depth);
	glScalef(this->scale, this->scale, this->scale);
	//glGetFloatv(GL_PROJECTION_MATRIX, this->projectionMatrix.data());

	// for Vertex Shader
	Matrix4f orthographicMat = Matrix4f({
		2 / (width * aspectRatio), 0, 0, 0,
		0, 2 / height, 0, 0,
		0, 0, -2 / depth, 0,
		0, 0, -1, 1
	});
	this->projectionMatrix = orthographicMat * MatrixFactory::scale<GLfloat>(this->scale);

	this->currentProjection = Projection::Orthographic;
	return *this;
}

Camera & Camera::setPerspectiveProjection() {
	return this->setPerspectiveProjection(this->fieldOfView);
}

Camera & Camera::setPerspectiveProjection(float fieldOfView) {
	this->fieldOfView = fieldOfView;

	float aspectRatio = (float) this->getAspectRatio();
	float zNear = 0.1f, zFar = 10000;

	// for OpenGL 1.0 commands
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(this->fieldOfView, aspectRatio, zNear, zFar);
	glScalef(this->scale, this->scale, this->scale);
	//glGetFloatv(GL_PROJECTION_MATRIX, this->projectionMatrix.data());

	// for Vertex Shader
	float radianFov = this->fieldOfView * (float) M_PI / 180;
	Matrix4f perspectiveMat = Matrix4f({
		1.0f / (std::tanf(radianFov / 2) * aspectRatio), 0, 0, 0,
		0, 1.0f / std::tanf(radianFov / 2), 0, 0,
		0, 0, (zNear + zFar) / (zNear - zFar), -1.f,
		0, 0, (2 * zNear * zFar) / (zNear - zFar), 0
	});
	this->projectionMatrix = perspectiveMat * MatrixFactory::scale<GLfloat>(this->scale);

	this->currentProjection = Projection::Perspective;
	return *this;
}

Camera & Camera::updateProjectionMatrix() {
	switch (this->currentProjection) {
	case Projection::Orthographic:
		this->setOrthographicProjection();
		break;
	case Projection::Perspective:
		this->setPerspectiveProjection(this->fieldOfView);
		break;
	}

	return *this;
}

Camera & Camera::setViewport(int width, int height) {
	this->viewport.width = width;
	this->viewport.height = height;
	glViewport(0, 0, width, height);
	this->updateProjectionMatrix();

	return *this;
}

double Camera::getAspectRatio() const {
	return this->viewport.width / (double) this->viewport.height;
}

void Camera::render(Model & model) {
	const ShaderProgram * shaderProgram;
	if (nullptr == (shaderProgram = model.getShaderProgram())) {
		return;
	}

	glUseProgram(shaderProgram->getProgramId());	// 셰이더 활성화

	// 셰이더 파라미터 설정
	GLint objectId;
	if (0 <= (objectId = glGetUniformLocation(shaderProgram->getProgramId(), "in_modelMatrix"))) {
		glUniformMatrix4fv(objectId, 1, GL_FALSE, model.getOwner().getWorldMatrix().data());
	}
	if (0 <= (objectId = glGetUniformLocation(shaderProgram->getProgramId(), "in_viewMatrix"))) {
		glUniformMatrix4fv(objectId, 1, GL_FALSE, this->viewMatrix.data());
	}
	if (0 <= (objectId = glGetUniformLocation(shaderProgram->getProgramId(), "in_projectionMatrix"))) {
		glUniformMatrix4fv(objectId, 1, GL_FALSE, this->projectionMatrix.data());
	}

	// 그리기
	model.draw(*shaderProgram);

	// 셰이더 비활성화
	glUseProgram(0);
}

void Camera::render(Actor & actor, bool recursive) {
	// render all model within actor
	std::vector<std::shared_ptr<Model>> models;
	if (!(models = actor.getComponents<Model>()).empty()) {
		for (std::shared_ptr<Model> & model : models) {
			this->render(*model);
		}
	}

	if (recursive) {
		for (std::shared_ptr<Actor> & child : actor) {
			this->render(*child, recursive);
		}
	}
}
