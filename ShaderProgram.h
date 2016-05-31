#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

#include <list>

#include "Shader.h"
#include "Exception.h"

class Model;

class ShaderProgram {
public:
	class Exception : public ::Exception {
	public:
		Exception(const std::string & message) : ::Exception(message) {};
	};
	class Recipe {
		friend class ShaderProgram;
		using ShaderT = std::list<Shader>;
		using Iterator = ShaderT::iterator;
		using ConstIterator = ShaderT::const_iterator;
	private:
		ShaderT shaders;

		GLuint link() const;
	public:
		Recipe & addShader(const Shader & shader);

		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;
	};
private:
	struct ShaderProgramIdHolder {
		GLuint programId;
		ShaderProgramIdHolder(GLuint programId);
		~ShaderProgramIdHolder();
	};
	std::shared_ptr<ShaderProgramIdHolder> programIdHolder;

	ShaderProgram(GLuint programId);
public:
	ShaderProgram(const Recipe & recipe);
	virtual ~ShaderProgram();

	GLuint getProgramId() const;
	virtual void onPreDraw(const Model & model) const;
	virtual void onPostDraw(const Model & model) const;
};
