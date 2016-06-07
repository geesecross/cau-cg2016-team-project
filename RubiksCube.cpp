#include "RubiksCube.h"
#include "Model.h"
#include "Resource.h"
#include <cmath>
#include "GameRule.h"

RubiksCube::RubiksCube(const size_t size, std::weak_ptr<AnimationManager> animationManager) : animationManager(animationManager), size(size) {
	this->blocks.resize(size);
	for (size_t x = 0; x < size; ++x) {
		this->blocks[x].resize(size);
		for (size_t y = 0; y < size; ++y) {
			this->blocks[x][y].resize(size);
			for (size_t z = 0; z < size; ++z) {
				this->blocks[x][y][z] = this->createChild<Block>();
			}
		}
	}

	this->cursor = this->createChild<Cursor>(std::ref(*this));

	this->resetBlocksAndCursor();
}

std::shared_ptr<RubiksCube::Cursor> RubiksCube::getCursor() {
	return this->cursor.lock();
}

void RubiksCube::twist(const size_t index, const Rotation & rotation) {
	auto ani = std::make_shared<TwistAnimation>(*this, index, rotation);
	bool empty = this->twistQueue.isEmpty();

	this->twistQueue.push([=](bool interrupted) {
		if (!ani->isStarted()) {
			this->animationManager.lock()->push(ani);
		}
		if (interrupted) {
			ani->interrupt();
		}

		return ani->isFinished();
	});

	if (empty) {
		// first entry
		this->twistQueue.execute();
	}
}

size_t RubiksCube::getSize() const {
	return this->size;
}

void RubiksCube::resetBlocksAndCursor() {
	this->twistQueue.clear();

	// reset block transforms
	const size_t size = this->size;
	float center = (size - 1) / 2.0f;
	for (size_t x = 0; x < size; ++x) {
		for (size_t y = 0; y < size; ++y) {
			for (size_t z = 0; z < size; ++z) {
				this->blocks[x][y][z].lock()->setTransform(
					Transform()
					.scalePost(Vector3f(0.9f))
					.translatePost({ (float)x - center, (float)y - center, (float)z - center })
				);
			}
		}
	}

	// reset cursor
	this->cursor.lock()->resetPositionAndRotation();
}

bool RubiksCube::isTwistFinished() const {
	return this->twistQueue.isEmpty();
}

TwistAnimation::TwistAnimation(RubiksCube & cube, size_t index, const Rotation & rotation) : cube(cube), index(index), rotation(rotation) {
	this->initialTransforms.resize(cube.size * cube.size);
}

void TwistAnimation::onStart() {
	const size_t size = this->cube.getSize();
	Vector3f axis = this->rotation.transform(Vector3f::yVector());
	Vector3f center = Vector3f((size - 1) / 2.0f);
	for (size_t x = 0; x < size; ++x) {
		for (size_t y = 0; y < size; ++y) {
			Vector3f indexVector = this->rotation.transform(Vector3f({ (float)x, (float)this->index, (float)y }) - center) + center + Vector3f(0.5f);
			size_t i = (size_t)indexVector[0], j = (size_t)indexVector[1], k = (size_t)indexVector[2];
			this->initialTransforms[x * size + y] = this->cube.blocks[i][j][k].lock()->getTransform();
		}
	}
}

bool TwistAnimation::stepFrame(const double timeElpased, const double timeDelta) {
	static const double degreesPerSec = 270;

	double angle = timeElpased * degreesPerSec;
	if (angle > 90)
		angle = 90;

	this->twist((float)angle);

	return angle >= 90;
}

void TwistAnimation::onFinished() {
	const size_t size = this->cube.getSize();
	Vector3f axis = this->rotation.transform(Vector3f::yVector());
	Vector3f center = Vector3f((size - 1) / 2.0f);
	RubiksCube::BlockArray tempArray;
	Rotation rot = this->rotation;
	rot.rotateByEuler(axis * 90);

	tempArray.resize(size);
	for (size_t x = 0; x < size; ++x) {
		tempArray[x].resize(size);
		for (size_t y = 0; y < size; ++y) {
			tempArray[x][y].resize(size);
			for (size_t z = 0; z < size; ++z) {
				tempArray[x][y][z] = this->cube.blocks[x][y][z];
			}
		}
	}

	for (size_t x = 0; x < size; ++x) {
		for (size_t z = 0; z < size; ++z) {
			Vector3f before = this->rotation.transform(Vector3f({ (float)x, (float)this->index, (float)z }) - center) + center + Vector3f(0.5f);
			size_t i = (size_t)before[0], j = (size_t)before[1], k = (size_t)before[2];
			Vector3f after = rot.transform(Vector3f({ (float)x, (float)this->index, (float)z }) - center) + center + Vector3f(0.5f);
			size_t a = (size_t)after[0], b = (size_t)after[1], c = (size_t)after[2];
			tempArray[a][b][c] = this->cube.blocks[i][j][k];
		}
	}

	for (size_t x = 0; x < size; ++x) {
		for (size_t y = 0; y < size; ++y) {
			for (size_t z = 0; z < size; ++z) {
				this->cube.blocks[x][y][z] = tempArray[x][y][z];
			}
		}
	}

	if (!this->isInterrupted()) {
		this->cube.twistQueue.execute();
		if (this->cube.isTwistFinished()) {
			this->cube.onFinishedTwist.raise();
		}
	}
}

void TwistAnimation::onInterruted() {
	this->twist(90);
}

void TwistAnimation::twist(const float angle) {
	const size_t size = this->cube.getSize();
	Vector3f axis = this->rotation.transform(Vector3f::yVector());
	Vector3f center = Vector3f((size - 1) / 2.0f);
	for (size_t x = 0; x < size; ++x) {
		for (size_t y = 0; y < size; ++y) {
			Vector3f indexVector = this->rotation.transform(Vector3f({ (float)x, (float)this->index, (float)y }) - center) + center + Vector3f(0.5f);
			size_t i = (size_t)indexVector[0], j = (size_t)indexVector[1], k = (size_t)indexVector[2];
			this->cube.blocks[i][j][k].lock()->setTransform(
				Transform(this->initialTransforms[x * size + y])
				.rotatePost(Rotation().rotateByEuler(axis * angle))
			);
		}
	}
}

void RubiksCube::Cursor::updateTransform() {
	this->setTransform(
		Transform()
		.scalePost(Vector3f(0.5f))
		.translatePost(this->position - Vector3f((this->cube.size - 1) / 2.0f))
		.rotatePost(this->rotation)
	);
}

RubiksCube::Cursor::Cursor(RubiksCube & cube) : cube(cube) {
	this->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Arrow])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Phong]);
	this->position = { (float)(cube.size / 2), (float)(cube.size / 2), (float)(cube.size - 1) };
	this->updateTransform();
}

RubiksCube::Cursor & RubiksCube::Cursor::move(const Vector2f & vector) {
	Vector3f up = this->getTransform().transformDirection(Vector3f::yVector()).normalized();
	Vector3f right = this->getTransform().transformDirection(Vector3f::xVector()).normalized();
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

	this->updateTransform();
	this->onCursorMoved.raise(*this, vector);
	return *this;
}

Vector3f RubiksCube::Cursor::getSelected() {
	return this->getTransform().transformPoint(Vector3f()) + Vector3f(1);
}

void RubiksCube::Cursor::twist(bool clockwise) {
	Rotation rot = this->rotation;
	size_t index = (size_t)this->position[1];
	if (clockwise) {
		rot.rotateByEuler(rot.transform(Vector3f::zVector()) * 180);
		index = this->cube.size - index - 1;
	}
	this->cube.twist(index, rot);
}

void RubiksCube::Cursor::rotateAxis(bool clockwise) {
	Vector3f front = this->getTransform().transformDirection(Vector3f::zVector()).normalized();
	this->rotation.rotateByEuler(front * (float)(90 * (clockwise ? -1 : 1)));
	if (clockwise) {
		float temp = this->position[0];
		this->position[0] = this->cube.getSize() - 1 - this->position[1];
		this->position[1] = temp;
	}
	else {
		float temp = this->position[0];
		this->position[0] = this->position[1];
		this->position[1] = this->cube.getSize() - 1 - temp;
	}
	this->updateTransform();
	this->onCursorRotated.raise(*this, clockwise);
}

void RubiksCube::Cursor::resetPositionAndRotation()
{
	this->position = { (float)(cube.size / 2), (float)(cube.size / 2), (float)(cube.size - 1) };
	this->rotation = Rotation();
	this->updateTransform();
}
