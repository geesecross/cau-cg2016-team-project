#define _USE_MATH_DEFINES

#include "Matrix.h"
#include "MatrixFactory.h"
#include "FileHelper.h"
#include "Model.h"

#include "Camera.h"

#include <cmath>

Camera::Camera() {
	this->viewport.width = 300;
	this->viewport.height = 300;
	this->setOrthographicProjection();
}

Camera & Camera::setTransform(const Transform & transform) {
	Actor::setTransform(transform);

	// for OpenGL 1.0
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(this->getViewMatrix().data());
	return *this;
}

Camera & Camera::setScale(GLfloat scale) {
	this->scale = scale;
	this->updateProjectionMatrix();
	return *this;
}

const Vector3f Camera::getViewReferencePoint() const {
	return this->transformPointToWorld(Vector3f::zeroVector());
}

const Vector3f Camera::getViewPlaneNormal() const {
	return this->transformDirectionToWorld(Vector3f::zVector());
}

Camera & Camera::move(const Vector3f & vector) {
	this->setTransform(
		Transform(this->getTransform())
		.translatePre(Vector3f(vector[0], vector[1], -vector[2]))
	);
	return *this;
}

Camera & Camera::rotate(const Rotation & rotation) {
	this->setTransform(
		Transform(this->getTransform())
		.rotatePre(rotation)
	);
	return *this;
}

const Matrix4f Camera::getViewMatrix() const {
	return this->getInvWorldMatrix();
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
		glUniformMatrix4fv(objectId, 1, GL_FALSE, this->getViewMatrix().data());
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
