#include <GL/glew.h>
#include <GL/freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "FileHelper.h"
#include "ShaderProgram.h"
#include "SimpleIlluminationModelShaderProgram.h"
#include "TransformFactory.h"
#include "Model.h"
#include "Camera.h"

float timeDelta = 0;
std::vector<Mesh *> meshes;
std::shared_ptr<ShaderProgram> shaderProgram;
size_t modelIndex = 0;
size_t lightMode = 0;
Camera * camera = nullptr;
bool debugMode = false;

float degree2radian(float degree) {
	return degree / 180.0f * (float)M_PI;
}

void drawExample1(float angle, Mesh * mesh, const ShaderProgram * shaderProgram) {
	static Model model;
	Matrix4f t2 = TransformFactory::translation<GLfloat>({ -400, 400, 0 });
	Matrix4f t1 = TransformFactory::translation<GLfloat>({ 0, 250 ,0 });
	Matrix4f r = TransformFactory::rotation(angle, TransformFactory::Z);

	model.bindMesh(mesh)
		.setColor({ 1, 0, 0, 1 })
		.bindShaderProgram(shaderProgram)
		.setTransformMatrix(t2 * r * t1);
	camera->render(model);
}

void drawExample2(float angle, Mesh * mesh, const ShaderProgram * shaderProgram) {
	static Model model;
	Matrix4f t2 = TransformFactory::translation<GLfloat>({ 400, -400, 0 });
	Matrix4f t1 = TransformFactory::translation<GLfloat>({ 0, 250 ,0 });
	Matrix4f r = TransformFactory::rotation(angle, TransformFactory::Z);

	model.bindMesh(mesh)
		.setColor({ 0, 1, 0, 1 })
		.bindShaderProgram(shaderProgram)
		.setTransformMatrix(t2 * t1 * r);
	camera->render(model);
}

void drawExample3(float angle, Mesh * mesh, const ShaderProgram * shaderProgram) {
	static Model model;
	float scale = 1 + cosf(angle * 8) * 0.5f;
	Matrix4f t2 = TransformFactory::translation<GLfloat>({ 400, 400, 0 });
	Matrix4f t1 = TransformFactory::translation<GLfloat>({ 0, 250 ,0 });
	Matrix4f s = TransformFactory::scale(scale);

	model.bindMesh(mesh)
		.setColor({ 0, 0, 1, 1 })
		.bindShaderProgram(shaderProgram)
		.setTransformMatrix(t2 * t1 * s);
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
	camera->setPerspectiveProjection(50);
	initCameraVectors();

	//meshes.push_back(new Mesh(Mesh::createFromDatFile("models/plane.txt")));
	meshes.push_back(new Mesh(Mesh::createFromDatFile("models/cube.dat")));
	meshes.push_back(new Mesh(Mesh::createFromDatFile("models/sphere.dat")));
	meshes.push_back(new Mesh(Mesh::createFromDatFile("models/teapot.dat")));
	meshes.push_back(new Mesh(Mesh::createFromDatFile("models/vase.dat")));
	meshes.push_back(new Mesh(Mesh::createFromDatFile("models/chair.dat")));

	shaderProgram.reset(new SimpleIlluminationModelShaderProgram(SimpleIlluminationModelShaderProgram::createPhong()));
	
	return true;
}

void uninit() {
	delete camera;
	for (Mesh * mesh : meshes) {
		delete mesh;
	}
}

void updateLightMode() {
	std::shared_ptr<SimpleIlluminationModelShaderProgram> program;
	if (program = std::dynamic_pointer_cast<SimpleIlluminationModelShaderProgram>(shaderProgram)) {
		switch (lightMode) {
		default:
		case 0:
			program->enableLightVectorAsPosition(true)
				.setLightVector(camera->getViewReferencePoint());
			break;
		case 1:
			program->enableLightVectorAsPosition(true)
				.setLightVector({ 1000, 1000, 1000 });
			break;
		case 2:
			program->enableLightVectorAsPosition(true)
				.setLightVector({ 1000, 0, 1000 });
			break;
		}
	}
}

void render() {
	Mesh * mesh = meshes[modelIndex];
	const ShaderProgram * shaderProgram = ::shaderProgram.get();
	updateLightMode();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float angle = 0;
	angle += degree2radian(timeDelta * 30);

	try {
		drawExample1(angle, mesh, shaderProgram);
		drawExample2(angle, mesh, shaderProgram);
		drawExample3(angle, mesh, shaderProgram);

		if (debugMode) {
			Model model = Model().bindMesh(mesh)
				.setColor({ 1, 1, 1, 1 })
				.bindShaderProgram(shaderProgram);
			camera->render(model);
		}
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
	float movingStep = 10, rotatingStep = 1;

	switch (ascii) {
	case ' ':
		modelIndex = (modelIndex + 1) % meshes.size();
		break;

	case '1':
	case '2':
	case '3':
		lightMode = ascii - '1';
		break;
		
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

	case 'G':
	case 'g':
		shaderProgram.reset(new SimpleIlluminationModelShaderProgram(SimpleIlluminationModelShaderProgram::createGouraud()));
		std::cout << "** Enabled Gouraud Shading" << std::endl;
		break;
	case 'H':
	case 'h':
		shaderProgram.reset(new SimpleIlluminationModelShaderProgram(SimpleIlluminationModelShaderProgram::createPhong()));
		std::cout << "** Enabled Phong Shading" << std::endl;
		break;

	case '.':
		std::cout << "VRP: " << camera->getViewReferencePoint() << std::endl;
		std::cout << "VPN: " << camera->getViewPlaneNormal() << std::endl;
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

void foo(Vector<float, 3> v) {
	std::cout << v << std::endl;
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
