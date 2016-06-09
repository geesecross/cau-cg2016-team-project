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
		{ 1, 1, 1, 1 },
		{ 0.25f, 0.80f, 1, 1 },
		{ 1, 1, 0, 1 },
		{ 0, 1, 1, 1 },
		{ 1, 0, 1, 1 },
	};

	for (std::pair<Vector3f, Vector3f> & position : positions) {
		std::shared_ptr<Actor> child = this->createChild<Actor>();
		child->setTransform(
			Transform(child->getTransform())
			.rotatePost(Rotation().rotateByEuler(position.second))
			.translatePost(position.first)
		);
		child->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Plane])
			.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Phong]);
	}

	for (size_t i = 0; i < 6; ++i) {
		(*this)[i]->getComponent<Model>()->setColor(colors[i]);
	}

	(*this)[0]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::TexturePng])
		.bindSpecularTexture(Resource::textures[Resource::Textures::TextureSpecular])
		.bindNormalTexture(Resource::textures[Resource::Textures::TextureNormal])
		.bindHeightTexture(Resource::textures[Resource::Textures::TextureHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Parallax]);
	(*this)[5]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::TexturePng])
		.bindSpecularTexture(Resource::textures[Resource::Textures::TextureSpecular])
		.bindNormalTexture(Resource::textures[Resource::Textures::TextureNormal])
		.bindHeightTexture(Resource::textures[Resource::Textures::TextureHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::NormalMap]);
	(*this)[2]->getComponent<Model>()->
		bindNormalTexture(Resource::textures[Resource::Textures::WaterNormal])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Water]);
	(*this)[1]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::Cobble])
		.bindNormalTexture(Resource::textures[Resource::Textures::CobbleNormal])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Cobble]);
}
