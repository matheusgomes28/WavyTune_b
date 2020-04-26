#ifndef SHADER_SHADER_PROGRAM_H
#define SHADER_SHADER_PROGRAM_H

// Includes from the STD
#include <string>

// Includes from third party
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>


class VertexShader;
class FragmentShader;
class GeometryShader;
class ShaderProgram
{

public:
	ShaderProgram(const std::string& vsPath, const std::string& fsPath);
	ShaderProgram(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath);
	~ShaderProgram();

	// Uniform setting stuff
	void setUniform(const std::string& name, const int& value) const;
	void setUniform(const std::string& name, const unsigned int& value) const;
	void setUniform(const std::string& name, const float& value) const;
	void setUniform(const std::string& name, const double& value) const;
	void setUniform(const std::string& name, const glm::vec2& value) const;
	void setUniform(const std::string& name, const glm::vec3& value) const;
	void setUniform(const std::string& name, const glm::mat4& value) const;

	void compileAndLink();
	void use();
	void unuse();

	GLuint getAddress() const;

private:

	// The shaders we have
	VertexShader* vs_;
	GeometryShader* gs_;
	FragmentShader* fs_;

	// Gl related stuff
	GLuint address_ = 0;

	GLuint generateAddress() const;
	void linkShaders();
};
#endif // SHADER_SHADER_PROGRAM_H
