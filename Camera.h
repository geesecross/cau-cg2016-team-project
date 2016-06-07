#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vector.h"
#include "Actor.h"
#include "Model.h"

class Camera : public Actor {
private:
	enum Projection {
		Orthographic,
		Perspective,
	};
	
	Matrix4f projectionMatrix;
	struct {
		int width, height;
	} viewport;
	GLfloat scale = 1;
	float fieldOfView = 60;
	Projection currentProjection;

	Camera & setProjectionMatrix(const Matrix4f & matrix);

public:
	Camera();

	virtual Camera & setTransform(const Transform & transform);

	Camera & setScale(GLfloat scale);
	const Vector3f getViewReferencePoint() const;
	const Vector3f getViewPlaneNormal() const;

	Camera & move(const Vector3f & vector);
	Camera & rotate(const Rotation & rotation);

	const Matrix4f getViewMatrix() const;
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