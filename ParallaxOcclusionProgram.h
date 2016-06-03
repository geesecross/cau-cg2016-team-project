#pragma once
#include "NormalMappingProgram.h"
#include "Model.h"
class ParallaxOcclusionProgram : public NormalMappingProgram {
private:
	ParallaxOcclusionProgram(Recipe & recipe);

public:
	static ParallaxOcclusionProgram create();


	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;
};