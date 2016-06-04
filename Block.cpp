#include "Block.h"

#include "Model.h"
#include "Resource.h"

Block::Block() {
	static std::pair<Vector3f, Vector3f> positions[] = {
		{ { 0, 0, 0.5 },{ 0, 0, 0 } },
		{ { 0, -0.5, 0 },{ 90, 0, 0 } },
		{ { 0.5, 0, 0 },{ 0, 90, 0 } },
		{ { 0, 0, -0.5 },{ 0, 180, 0 } },
		{ { 0, 0.5, 0 },{ -90, 0, 0 } },
		{ { -0.5, 0, 0 },{ 0, -90, 0 } },
	};

	static Vector4f colors[] = {
		{ 1, 1, 1, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 1, 1, 0, 1 },
		{ 0, 1, 1, 1 },
		{ 1, 0, 1, 1 },
	};

	for (std::pair<Vector3f, Vector3f> & position : positions) {
		std::shared_ptr<Actor> child = this->createChild<Actor>();
		child->getTransform()
			.rotatePost(Rotation().rotateByEuler(position.second))
			.translatePost(position.first);
		child->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Plane])
			.bindShaderProgram(Resource::shaderPrograms[Resource::Phong]);
	}

	for (size_t i = 0; i < 6; ++i) {
		(*this)[i]->getComponent<Model>()->setColor(colors[i]);
	}

	(*this)[2]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::TexturePng])
		.bindSpecularTexture(Resource::textures[Resource::TextureSpecular])
		.bindNormalTexture(Resource::textures[Resource::TextureNormal])
		.bindHeightTexture(Resource::textures[Resource::TextureHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::Parallax]);
	(*this)[5]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::TexturePng])
		.bindSpecularTexture(Resource::textures[Resource::TextureSpecular])
		.bindNormalTexture(Resource::textures[Resource::TextureNormal])
		.bindHeightTexture(Resource::textures[Resource::TextureHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::NormalMap]);
	(*this)[0]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::TexturePng])
		.bindSpecularTexture(Resource::textures[Resource::TextureSpecular])
		.bindNormalTexture(Resource::textures[Resource::TextureNormal])
		.bindHeightTexture(Resource::textures[Resource::TextureHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::MovingTexture]);
}
