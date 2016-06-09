#include <GL/glew.h>
#include <GL/freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>

#include "FileHelper.h"
#include "ShaderProgram.h"
#include "SimpleIlluminationModelShaderProgram.h"
#include "MatrixFactory.h"
#include "Model.h"
#include "Camera.h"
#include "RubiksCube.h"
#include "Animation.h"
#include "MovingTexShader.h"

#include "Resource.h"
#include "GameRule.h"

float timeDelta = 0;
const size_t cube_size = 3; // 큐브의 한 열의 블럭 개수
std::shared_ptr<Camera> camera;
std::shared_ptr<Actor> skybox, sea;
std::shared_ptr<RubiksCube> rubiksCube;
std::shared_ptr<AnimationManager> animationManager;
std::shared_ptr<GameRule> gameRule;
bool debugMode = false;

float degree2radian(float degree) {
	return degree / 180.0f * (float)M_PI;
}

void initCameraVectors() {
	camera->setTransform(
		Transform().translatePost({ 0, 0, 10 })
	);
}

bool init() {
	try {
		if (GLEW_OK != glewInit()) {
			std::cerr << "Error: glewInit()" << std::endl;
			return false;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

		Resource::init();
		animationManager.reset(new AnimationManager());
		camera.reset(new Camera());
		camera->setPerspectiveProjection(50);
		initCameraVectors();

		animationManager->push(std::make_shared<MovingTextureAnimation>(*static_cast<MovingTexShader *>(Resource::shaderPrograms[Resource::ShaderPrograms::Water])));
		animationManager->push(std::make_shared<MovingTextureAnimation>(*static_cast<MovingTexShader *>(Resource::shaderPrograms[Resource::ShaderPrograms::Sea])));

		rubiksCube.reset(new RubiksCube(cube_size, animationManager));
		gameRule.reset(new GameRule(rubiksCube, animationManager, camera));
		skybox.reset(new Actor());
		skybox->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Skybox])
			.bindDiffuseTexture(Resource::textures[Resource::Textures::Skybox])
			.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Skybox]);
		sea.reset(new Actor());
		sea->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Plane])
			.setColor({ 112 / 255.f, 181 / 255.f, 213 / 255.f, 0.8f })
			.bindDiffuseTexture(Resource::textures[Resource::Textures::AlphaCircle])
			.bindNormalTexture(Resource::textures[Resource::Textures::WaterNormal])
			.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Sea]);
	}
	catch (Exception & e) {
		std::cout << e.what() << std::endl;
		std::cin >> std::string();
		return true;
	}

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
		actor.createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Blockside])
			.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Phong])
			.setColor({ 1, 1, 1, 1 });
		auto child = actor.createChild<Actor>();
		child->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Blockside])
			.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Phong])
			.setColor({ 1, 0, 0, 1 });
		child->setTransform(
			Transform()
			.rotatePost(Rotation().rotateByEuler({}))
		);
		camera->render(actor);*/

		camera->render(*rubiksCube);
		skybox->setTransform(
			Transform()
			.scalePost(Vector3f(2.5f))
			//.rotatePost(Rotation().rotateByEuler({ 0, 90, 0 }))
			.translatePost({ 0, -20, 0 })
			.translatePost(camera->getViewReferencePoint())
		);
		camera->render(*skybox);
		sea->setTransform(
			Transform()
			.scalePost({ 500, 500, 1 })
			.rotatePost(Rotation().rotateByEuler(Vector3f::xVector() * -90))
			.translatePost({ 0, -20, 0 })
			.translatePost(camera->getViewReferencePoint())
		);
		camera->render(*sea);
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

	gameRule->step();

	if (0 < timeDelta) {
		previousTime = currentTime;
		glutPostRedisplay();
	}
}

void onResize(int width, int height) {
	camera->setViewport(width, height);
}

void onKeyboard(unsigned char ascii, int x, int y) {
	static int prevX = x, prevY = y;
	float movingStep = 0.1f, rotatingStep = 1;
	

	switch (ascii) {
	case '1':
		camera->setTransform(
			Transform(camera->getTransform())
			.rotatePre(Rotation().rotateByEuler({ 0.5f * (prevY - y), 0.5f * (prevX - x), 0 }))
		);
		break;
	/*case 'O':
	case 'o':
		camera->setOrthographicProjection();
		std::cout << "** Enabled Orthographic Projection" << std::endl;
		break;*/
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
		camera->rotate(Rotation().rotateByEuler(Vector3f::yVector() * rotatingStep));
		break;

	case 'E':
		rotatingStep *= 10;
	case 'e':
		camera->rotate(Rotation().rotateByEuler(Vector3f::yVector() * -rotatingStep));
		break;

	case '.':
		std::cout << "ViewMat2: " << camera->getTransform().getInvMatrix() << std::endl;
		std::cout << "ViewMat: " << camera->getViewMatrix() << std::endl;
		std::cout << "VRP: " << camera->getViewReferencePoint() << std::endl;
		std::cout << "VPN: " << camera->getViewPlaneNormal() << std::endl;
		break;

	case 'i':
		gameRule->move(Vector2f({ 0, 1 }) * 1);
		break;
	case 'k':
		gameRule->move(Vector2f({ 0, 1 }) * -1);
		break;
	case 'j':
		gameRule->move(Vector2f({ 1, 0 }) * -1);
		break;
	case 'l':
		gameRule->move(Vector2f({ 1, 0 }) * 1);
		break;
	case ';':
		std::cout << rubiksCube->getCursor()->getSelected() << std::endl;
		break;
	case 'z':
		gameRule->twist(true);
		break;
	case 'x':
		gameRule->twist(false);
		break;
	case 'c':
		gameRule->rotateAxis(true);
		break;

	case 13:
		initCameraVectors();
		std::cout << "** Resetted Camera Position and Rotation" << std::endl;
		break;
	case ' ':
		camera->setTransform(
			Transform(rubiksCube->getCursor()->getWorldTransform())
			.translatePre({ 0, 0, 10 })
		);
		break;
	case '`':
		debugMode = !debugMode;
		gameRule->toggleDebugMode();
		break;
	}
	prevX = x;
	prevY = y;

	glutPostRedisplay();
}

void onKeyboardUp(unsigned char ascii, int x, int y) {
	switch(ascii)
	{
		case 't':
			gameRule->resetGame();
			break;
		case 'y':
			gameRule->scramble();
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
	glutKeyboardUpFunc(onKeyboardUp);
	glutReshapeFunc(onResize);

	if (init()) {
		glutMainLoop();
		uninit();
	}

	return 0;
}
