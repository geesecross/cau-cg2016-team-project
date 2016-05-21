#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vector.h"
#include "Model.h"

class Camera {
private:
	enum Projection {
		Orthographic,
		Perspective,
	};

	static unsigned int referenceCount;
	static GLuint transformProgramId;
	static GLuint modelMatrixId, viewMatrixId, projectionMatrixId, vertexPositionId, fragColorId;

	Matrix4f viewMatrix, projectionMatrix;
	Vector3f viewReferencePoint, viewPlaneNormal, viewUpVector;
	struct {
		int width, height;
	} viewport;
	GLfloat scale = 1;
	float fieldOfView = 60;
	Projection currentProjection;

	static void initShader();
	static void uninitShader();

	void updateViewMatrix();
	Camera & setViewMatrix(const Matrix4f & matrix);
	Camera & setProjectionMatrix(const Matrix4f & matrix);

public:
	Camera();
	virtual ~Camera();

	Camera & setScale(GLfloat scale);
	const Vector3f & getViewReferencePoint() const;
	Camera & setViewReferencePoint(const Vector3f & point);
	Camera & setViewPlaneNormal(const Vector3f & viewPlaneNormal);

	Camera & move(const Vector3f & vector);
	Camera & rotate(const float radian);

	const Matrix4f & getViewMatrix() const;
	const Matrix4f & getProjectionMatrix() const;

	Camera & setOrthographicProjection();
	Camera & setPerspectiveProjection(float fieldOfView);
	Camera & updateProjectionMatrix();
	Camera & setViewport(int width, int height);

	double getAspectRatio() const;

	void render(const Model & model);
};