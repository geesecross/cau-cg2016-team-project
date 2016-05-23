#include <GL/glew.h>
#include <GL/freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "TransformBuilder.h"
#include "Model.h"
#include "Camera.h"

float timeDelta = 0;
std::vector<Mesh *> meshes;
size_t modelIndex = 0;
Camera * camera = nullptr;

float degree2radian(float degree) {
	return degree / 180.0f * (float)M_PI;
}

void drawExample1(float angle, Mesh * mesh)
{
	Matrix4f t2 = TransformBuilder::buildTranslation<GLfloat>({ -400, 400, 0 });
	Matrix4f t1 = TransformBuilder::buildTranslation<GLfloat>({ 0, 250 ,0 });
	Matrix4f r = TransformBuilder::buildRotation(angle, TransformBuilder::Z);

	Model model = Model().setMesh(mesh)
		.setTransformMatrix(t2 * r * t1)
		.setColor({ 1, 0, 0, 1 });
	camera->render(model);
}

void drawExample2(float angle, Mesh * mesh)
{
	Matrix4f t2 = TransformBuilder::buildTranslation<GLfloat>({ 400, -400, 0 });
	Matrix4f t1 = TransformBuilder::buildTranslation<GLfloat>({ 0, 250 ,0 });
	Matrix4f r = TransformBuilder::buildRotation(angle, TransformBuilder::Z);

	Model model = Model().setMesh(mesh)
		.setTransformMatrix(t2 * t1 * r)
		.setColor(Vector4f({ 0, 1, 0, 1 }));
	camera->render(model);
}

void drawExample3(float angle, Mesh * mesh)
{
	float scale = 1 + cosf(angle * 8) * 0.5f;
	Matrix4f t2 = TransformBuilder::buildTranslation<GLfloat>({ 400, 400, 0 });
	Matrix4f t1 = TransformBuilder::buildTranslation<GLfloat>({ 0, 250 ,0 });
	Matrix4f s = TransformBuilder::buildScale(scale);

	Model model = Model().setMesh(mesh)
		.setTransformMatrix(t2 * t1 * s)
		.setColor({ 0, 0, 1, 1 });
	camera->render(model);
}

void initCameraVectors() {
	camera->setViewReferencePoint({ 0, 0, 2000 })
		.setViewPlaneNormal({ 0, 0, 1 });
}

bool init() {
	if (GLEW_OK != glewInit()) {
		std::cerr << "Error: glewInit()" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	camera = new Camera();
	camera->setOrthographicProjection();
	initCameraVectors();

	meshes.push_back(Mesh::createFromDatFile("models/teapot.dat"));
	meshes.push_back(Mesh::createFromDatFile("models/vase.dat"));
	meshes.push_back(Mesh::createFromDatFile("models/chair.dat"));
	meshes.push_back(Mesh::createFromDatFile("models/cube.dat"));
	meshes.push_back(Mesh::createFromDatFile("models/sphere.dat"));

	return true;
}

void uninit() {
	delete camera;
}

void render() {
	Mesh * mesh = meshes[modelIndex];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float angle = 0;
	angle += degree2radian(timeDelta * 30);

	drawExample1(angle, mesh);
	drawExample2(angle, mesh);
	drawExample3(angle, mesh);

	glutSwapBuffers();
}

void idle() {
	static long previousTime = glutGet(GLUT_ELAPSED_TIME);

	long currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDelta = (currentTime - previousTime) / 1000.0f;

	if (0 < timeDelta) {
		previousTime = currentTime;
		glutPostRedisplay();
	}
}

void onResize(int width, int height) {
	camera->setViewport(width, height);
}

void onKeyboard(unsigned char ascii, int x, int y) {
	float movingStep = 10, rotatingStep = 5;

	switch (ascii) {
	case ' ':
		modelIndex = (modelIndex + 1) % meshes.size();
		break;
		
	case 'O':
	case 'o':
		camera->setOrthographicProjection();
		break;
	case 'P':
	case 'p':
		camera->setPerspectiveProjection(60);
		break;

	case 'W':
		movingStep *= 10;
	case 'w':
		camera->move({ 0, 0, movingStep });
		break;
	case 'S':
		movingStep *= 10;
	case 's':
		camera->move({ 0, 0, -movingStep });
		break;

	case 'A':
		movingStep *= 10;
	case 'a':
		camera->move({ -movingStep, 0, 0 });
		break;
	case 'D':
		movingStep *= 10;
	case 'd':
		camera->move({ movingStep, 0, 0 });
		break;

	case 'R':
		movingStep *= 10;
	case 'r':
		camera->move({ 0, movingStep, 0 });
		break;
	case 'F':
		movingStep *= 10;
	case 'f':
		camera->move({ 0, -movingStep, 0 });
		break;

	case 'Q':
		rotatingStep *= 3;
	case 'q':
		camera->rotate(degree2radian(rotatingStep));
		break;

	case 'E':
		rotatingStep *= 3;
	case 'e':
		camera->rotate(degree2radian(-rotatingStep));
		break;

	case 13:
		initCameraVectors();
		break;
	}
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("20121955_HW02");
	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutKeyboardFunc(onKeyboard);
	glutReshapeFunc(onResize);

	if (init()) {
		glutMainLoop();
		uninit();
	}

	return 0;
}
