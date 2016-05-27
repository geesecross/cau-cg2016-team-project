#include "RubiksCube.h"
#include "Model.h"
#include "Resource.h"

RubiksCube::RubiksCube(const size_t size, std::weak_ptr<AnimationManager> animationManager) : animationManager(animationManager), size(size) {
	float center = (size - 1) / 2.0f;
	this->blocks.resize(size);
	for (size_t x = 0; x < size; ++x) {
		this->blocks[x].resize(size);
		for (size_t y = 0; y < size; ++y) {
			this->blocks[x][y].resize(size);
			for (size_t z = 0; z < size; ++z) {
				this->blocks[x][y][z] = this->createChild<Block>();
				this->blocks[x][y][z].lock()->
					scale(Vector3f(0.9f))
					.moveOrigin(Vector3f((float)x - center, (float)y - center, (float)z - center));
			}
		}
	}

	this->cursor = this->createChild<Cursor>(std::ref(*this));
}

std::shared_ptr<RubiksCube::Cursor> RubiksCube::getCursor() {
	return this->cursor.lock();
}

void RubiksCube::twist(const size_t index, const Axis axis) {
	auto ani = std::make_shared<TwistAnimation>(*this, index, axis);
	bool empty = this->commandQueue.empty();

	this->commandQueue.push([=] {
		if (!ani->isStarted()) {
			this->animationManager.lock()->push(ani);
		}
		return ani->isFinished();
	});

	if (empty) {
		// first entry
		this->commandQueue.execute();
	}
}

TwistAnimation::TwistAnimation(RubiksCube & cube, size_t index, RubiksCube::Axis axis) : cube(cube), index(index), axis(axis) {
}

bool TwistAnimation::stepFrame(const double timeElpased, const double timeDelta) {	// todo
	static const double degreesPerSec = 180;
	const size_t size = this->cube.blocks.size();
	double angle = timeDelta * degreesPerSec;
	if ((timeElpased * degreesPerSec) > 90)
		angle = 90 - (timeElpased - timeDelta) * degreesPerSec;

	static const Vector3f steps[] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	};
	static const Vector3f selections[] = {
		{ 0, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
	};

	Vector3f step = steps[axis];
	Vector3f selection = selections[axis];

	for (size_t x = (size_t)step[RubiksCube::X] * index; x < size; x += step[RubiksCube::X] == 0 ? 1 : size) {
		for (size_t y = (size_t)step[RubiksCube::Y] * index; y < size; y += step[RubiksCube::Y] == 0 ? 1 : size) {
			for (size_t z = (size_t)step[RubiksCube::Z] * index; z < size; z += step[RubiksCube::Z] == 0 ? 1 : size) {
				this->cube.blocks[x][y][z].lock()->rotate(Rotation().rotateByEuler(step * (float)angle));
			}
		}
	}

	return (timeElpased * degreesPerSec) >= 90;
}

void TwistAnimation::onFinished() {
	this->cube.commandQueue.execute();
}

void RubiksCube::Cursor::updateTransform() {
	this->resetTransform()
		.scale(Vector3f(0.5f))
		.moveOrigin(this->position - Vector3f((float)(this->cube.size / 2)))
		.rotate(this->rotation);
}

RubiksCube::Cursor::Cursor(RubiksCube & cube) : cube(cube) {
	this->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Arrow])
		.bindShaderProgram(Resource::shaderPrograms[Resource::Phong]);
	this->position = { (float)(cube.size / 2), (float)(cube.size / 2), (float)(cube.size - 1) };
	this->updateTransform();
}

RubiksCube::Cursor & RubiksCube::Cursor::move(const Vector2f & vector) {
	Vector3f up = this->transformDirection(Vector3f::yVector()).normalized();
	Vector3f right = this->transformDirection(Vector3f::xVector()).normalized();
	this->position += Vector3f::xVector() * vector[0] + Vector3f::yVector() * vector[1];

	// bounding
	if (this->position[0] < 0) {
		this->position[0] = (float)(this->cube.size + this->position[0]);
		this->rotation.rotateByEuler(up * -90);
	}
	if (this->position[0] >= this->cube.size) {
		this->position[0] = 0;
		this->rotation.rotateByEuler(up * 90);
	}

	if (this->position[1] < 0) {
		this->position[1] = (float)(this->cube.size + this->position[1]);
		this->rotation.rotateByEuler(right * 90);
	}
	if (this->position[1] >= this->cube.size) {
		this->position[1] = 0;
		this->rotation.rotateByEuler(right * -90);
	}

	if (this->position[2] < 0) {
		this->position[2] = (float)(this->cube.size + this->position[2]);
	}
	if (this->position[2] >= this->cube.size) {
		this->position[2] = 0;
	}

	this->updateTransform();
	return *this;
}

Vector3f RubiksCube::Cursor::getSelected() {
	return this->transformPoint(Vector3f()) + Vector3f(1);
}

RubiksCube::Axis RubiksCube::Cursor::getAxis() {
	Vector3f up = this->transformDirection(Vector3f::yVector());
	if (0 != up[0])
		return Axis::X;
	if (0 != up[1])
		return Axis::Y;
	return Axis::Z;
}

void RubiksCube::Cursor::twist(bool clockWise) {
	Vector3f selected = this->getSelected();
	//this->cube.twist(selected[this->getAxis()], this->getAxis());
}
