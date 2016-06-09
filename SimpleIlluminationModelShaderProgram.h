#pragma once
#include "ShaderProgram.h"
#include "Vector.h"

class SimpleIlluminationModelShaderProgram : public ShaderProgram {
private:
	GLfloat ambientRatio = 0.4f, diffusionRatio = 0.4f, specularRatio = 0.2f, shiness = 128;
	Vector3f lightVector;
	bool enabledLightVectorAsPosition = false;

protected:
	SimpleIlluminationModelShaderProgram(Recipe & recipe);
public:
	static SimpleIlluminationModelShaderProgram createPhong();
	static SimpleIlluminationModelShaderProgram createGouraud();

	SimpleIlluminationModelShaderProgram & setAmbientRatio(GLfloat ratio);
	GLfloat getAmbientRatio() const;
	SimpleIlluminationModelShaderProgram & setDiffusionRatio(GLfloat ratio);
	GLfloat getDiffusionRatio() const;
	SimpleIlluminationModelShaderProgram & setSpecularRatio(GLfloat ratio);
	GLfloat getSpecularRatio() const;
	SimpleIlluminationModelShaderProgram & setShiness(GLfloat shiness);
	GLfloat getShiness() const;

	SimpleIlluminationModelShaderProgram & setLightVector(const Vector3f & vector);
	SimpleIlluminationModelShaderProgram & enableLightVectorAsPosition(bool enabled);
	
	virtual void onPreDraw(const Model & model) const;
};
