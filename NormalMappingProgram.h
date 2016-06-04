#pragma once
#include "TextureShaderProgram.h"
#include "Vector.h"
class NormalMappingProgram : public TextureShaderProgram {
public: 
	NormalMappingProgram(Recipe & recipe);
	static NormalMappingProgram create();


	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;

};