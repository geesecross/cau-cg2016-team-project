#include <GL/glew.h>
#include <GL/freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>

#include "FileHelper.h"
#include "ShaderProgram.h"
#include "SimpleIlluminationModelShaderProgram.h"
#include "TransformFactory.h"
#include "Model.h"
#include "Camera.h"
#include "RubiksCube.h"
#include "Animation.h"

#include "Resource.h"

float timeDelta = 0;
size_t modelIndex = 0;
size_t lightMode = 0;
std::shared_ptr<Camera> camera;
std::shared_ptr<RubiksCube> rubiksCube;
std::shared_ptr<AnimationManager> animationManager;
bool debugMode = false;

float degree2radian(float degree) {
	return degree / 180.0f * (float)M_PI;
}

void initCameraVectors() {
	camera->setViewReferencePoint({ 0, 0, 5 })
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

	Resource::init();
	animationManager.reset(new AnimationManager());
	camera.reset(new Camera());
	camera->setPerspectiveProjection(50);
	initCameraVectors();

	rubiksCube.reset(new RubiksCube(3, animationManager));

	return true;
}

void uninit() {
	Resource::uninit();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float phase = 0;
	phase += timeDelta;

	try {
		animationManager->step(timeDelta);
		/*Actor actor;
		actor.createComponent<Model>()->bindMesh(Resource::meshes[Resource::Plane])
			.setColor({ 1, 1, 1, 1 });
		camera->render(actor);*/
		camera->render(*rubiksCube, true);
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
	}

	glutSwapBuffers();
}

void onIdle() {
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
	float movingStep = 1, rotatingStep = 1;
	static RubiksCube::Axis axis = RubiksCube::X;

	switch (ascii) {
	case 'O':
	case 'o':
		camera->setOrthographicProjection();
		std::cout << "** Enabled Orthographic Projection" << std::endl;
		break;
	case 'P':
	case 'p':
		camera->setPerspectiveProjection();
		std::cout << "** Enabled Perspective Projection" << std::endl;
		break;

	case 'W':
		movingStep *= 10;
	case 'w':
		camera->move(Vector3f::zVector() * movingStep);
		break;
	case 'S':
		movingStep *= 10;
	case 's':
		camera->move(Vector3f::zVector() * -movingStep);
		break;

	case 'A':
		movingStep *= 10;
	case 'a':
		camera->move(Vector3f::xVector() * -movingStep);
		break;
	case 'D':
		movingStep *= 10;
	case 'd':
		camera->move(Vector3f::xVector() * movingStep);
		break;

	case 'R':
		movingStep *= 10;
	case 'r':
		camera->move(Vector3f::yVector() * movingStep);
		break;
	case 'F':
		movingStep *= 10;
	case 'f':
		camera->move(Vector3f::yVector() * -movingStep);
		break;

	case 'Q':
		rotatingStep *= 10;
	case 'q':
		camera->rotate(degree2radian(rotatingStep));
		break;

	case 'E':
		rotatingStep *= 10;
	case 'e':
		camera->rotate(degree2radian(-rotatingStep));
		break;

	case '.':
		std::cout << "VRP: " << camera->getViewReferencePoint() << std::endl;
		std::cout << "VPN: " << camera->getViewPlaneNormal() << std::endl;
		break;

	case 'i':
		rubiksCube->getCursor()->move(Vector2f({ 0, 1 }) * 1);
		break;
	case 'k':
		rubiksCube->getCursor()->move(Vector2f({ 0, 1 }) * -1);
		break;
	case 'j':
		rubiksCube->getCursor()->move(Vector2f({ 1, 0 }) * -1);
		break;
	case 'l':
		rubiksCube->getCursor()->move(Vector2f({ 1, 0 }) * 1);
		break;
	case ';':
		std::cout << rubiksCube->getCursor()->getSelected() << std::endl;
		break;
	case 'z':
		//rubiksCube->twist();
		break;
	case 'x':
		break;

	case 13:
		initCameraVectors();
		std::cout << "** Resetted Camera Position and Rotation" << std::endl;
		break;
	case '`':
		debugMode = !debugMode;
		break;
	}
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("20121955_HW03");
	glutDisplayFunc(render);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutReshapeFunc(onResize);

	if (init()) {
		glutMainLoop();
		uninit();
	}

	return 0;
}
