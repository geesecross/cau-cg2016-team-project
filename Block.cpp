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
		/*{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 },
		{ 0.6f, 0.2f, 0.6f, 1 },
		{ 1, 1, 1, 1 },
		{ 0.20f, 0.75f, 1, 1 },
		{ 1, 1, 1, 1 }*/
		{ 1, 1, 1, 1 },
		{ 1, 0.8f, 0.8f, 1 },
		{ 0.8f, 0.8f, 1.f, 1 },
		{ 0.8f, 1, 0.8f, 1 },
		{ 0.20f, 0.75f, 1, 1 },
		{ 1, 1, 0.8f, 1 }
	};

	for (std::pair<Vector3f, Vector3f> & position : positions) {
		std::shared_ptr<Actor> child = this->createChild<Actor>();
		child->setTransform(
			Transform(child->getTransform())
			.rotatePost(Rotation().rotateByEuler(position.second))
			.translatePost(position.first)
		);
		child->createComponent<Model>()->bindMesh(Resource::meshes[Resource::Meshes::Blockside])
			.bindDiffuseTexture(Resource::textures[Resource::Textures::Metal])
			.bindNormalTexture(Resource::textures[Resource::Textures::MetalNormal])
			.bindHeightTexture(Resource::textures[Resource::Textures::MetalHeight])
			.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::MetalParallax]);
	}

	for (size_t i = 0; i < 6; ++i) {
		(*this)[i]->getComponent<Model>()->setColor(colors[i]);
	}

	/*(*this)[0]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::Metal])
		.bindNormalTexture(Resource::textures[Resource::Textures::MetalNormal])
		.bindHeightTexture(Resource::textures[Resource::Textures::MetalHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::MetalParallax]);
	(*this)[1]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::Cobble])
		.bindSpecularTexture(Resource::textures[Resource::Textures::CobbleSpecular])
		.bindNormalTexture(Resource::textures[Resource::Textures::CobbleNormal])
		.bindHeightTexture(Resource::textures[Resource::Textures::CobbleHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Cobble]);
	(*this)[2]->getComponent<Model>()->
		//bindDiffuseTexture(Resource::textures[Resource::Textures::Cushion])
		bindNormalTexture(Resource::textures[Resource::Textures::CushionNormal])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Cushion]);
	(*this)[3]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::Wood])
		.bindNormalTexture(Resource::textures[Resource::Textures::WoodNormal])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Wood]);*/
	(*this)[4]->getComponent<Model>()->
		bindDiffuseTexture(nullptr)
		.bindNormalTexture(Resource::textures[Resource::Textures::WaterNormal])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::Water]);
	(*this)[5]->getComponent<Model>()->
		bindDiffuseTexture(Resource::textures[Resource::Textures::Metal])
		.bindNormalTexture(Resource::textures[Resource::Textures::MetalNormal])
		.bindHeightTexture(Resource::textures[Resource::Textures::MetalHeight])
		.bindShaderProgram(Resource::shaderPrograms[Resource::ShaderPrograms::MetalNormal]);
}
