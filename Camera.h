#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vector.h"
#include "Actor.h"
#include "Model.h"

class Camera {
private:
	enum Projection {
		Orthographic,
		Perspective,
	};

	Matrix4f viewMatrix, projectionMatrix;
	Vector3f viewReferencePoint, viewPlaneNormal, viewUpVector;
	struct {
		int width, height;
	} viewport;
	GLfloat scale = 1;
	float fieldOfView = 60;
	Projection currentProjection;

	void updateViewMatrix();
	Camera & setViewMatrix(const Matrix4f & matrix);
	Camera & setProjectionMatrix(const Matrix4f & matrix);

public:
	Camera();

	Camera & setScale(GLfloat scale);
	const Vector3f & getViewReferencePoint() const;
	Camera & setViewReferencePoint(const Vector3f & point);
	const Vector3f & getViewPlaneNormal() const;
	Camera & setViewPlaneNormal(const Vector3f & viewPlaneNormal);

	Camera & move(const Vector3f & vector);
	Camera & rotate(const float radian);

	const Matrix4f & getViewMatrix() const;
	const Matrix4f & getProjectionMatrix() const;

	Camera & setOrthographicProjection();
	Camera & setPerspectiveProjection();
	Camera & setPerspectiveProjection(float fieldOfView);
	Camera & updateProjectionMatrix();
	Camera & setViewport(int width, int height);

	double getAspectRatio() const;

	void render(Model & model);
	void render(Actor & actor, bool recursive = true);
};