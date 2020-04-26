// Includes from this project
#include "pch.h"
#include "shader_program.h"
#include "vertex_shader.h"
#include "fragment_shader.h"
#include "geometry_shader.h"

// Includes from the STD
#include <exception>
#include <iostream>

// Includes from third party
#include <glm/gtc/type_ptr.hpp>


ShaderProgram::ShaderProgram(const std::string& vsPath, const std::string& fsPath)
	: vs_(new VertexShader(vsPath)),
	fs_(new FragmentShader(fsPath)),
	gs_(nullptr)
{}

ShaderProgram::ShaderProgram(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath)
	: vs_(new VertexShader(vsPath)),
	fs_(new FragmentShader(fsPath)),
	gs_(new GeometryShader(gsPath))
{}


GLuint ShaderProgram::generateAddress() const
{
	return glCreateProgram();
}

void ShaderProgram::compileAndLink()
{
	// Generate the address for this program
	address_ = generateAddress();

	// Compile all the shaders, generating the
	// OpenGL address_
	vs_->compile();
	if (gs_) {
		gs_->compile();
	}
	fs_->compile();

	std::cout << "Vertex Shader: " << vs_->getCompileMessage() << std::endl;
	std::cout << "Fragment Shader: " << fs_->getCompileMessage() << std::endl;
	if (gs_) std::cout << "Geometry Shader: " << gs_->getCompileMessage() << std::endl;

	// Link all the shaders
	linkShaders();
}

void ShaderProgram::use()
{
	if (address_ != 0) {
		glUseProgram(address_);
	}
	else {
		throw std::logic_error("Cannot use uncompiled shader program.");
	}
}

void ShaderProgram::unuse()
{
}

void ShaderProgram::linkShaders()
{
	if (address_ > 0) {
		// Need to attach all the addresses
		glAttachShader(address_, vs_->getAddress());
		if (gs_) {
			glAttachShader(address_, gs_->getAddress());
		}
		glAttachShader(address_, fs_->getAddress());
		glLinkProgram(address_);
	}
	else {
		throw std::logic_error("Address not created created for shader program");
	}
}

GLuint ShaderProgram::getAddress() const
{
	return address_;
}

void ShaderProgram::setUniform(const std::string& name, const int& value) const
{
	if (address_ > 0) {
		// Get the location of the uniform then set it
		int uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniform1i(uniformLoc, value);
	}
}

void ShaderProgram::setUniform(const std::string& name, const unsigned int& value) const
{
	if (address_ > 0) {
		// Get the location of the uniform then set it
		int uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniform1ui(uniformLoc, value);
	}
}

void ShaderProgram::setUniform(const std::string& name, const float& value) const
{
	if (address_ > 0) {
		int uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniform1f(uniformLoc, value);
	}
}

void ShaderProgram::setUniform(const std::string& name, const double& value) const
{
	if (address_ > 0) {
		int uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniform1d(uniformLoc, value);
	}
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) const
{
	if (address_ > 0) {
		int uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniform2f(uniformLoc, value.x, value.y);
	}
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const
{
	if (address_ > 0) {
		int uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniform3f(uniformLoc, value.x, value.y, value.z);
	}
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) const
{
	if (address_ > 0) {
		GLint uniformLoc = glGetUniformLocation(address_, name.c_str());
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
	}
}


ShaderProgram::~ShaderProgram()
{
	// Simple deletion of the pointers
	if (vs_) {
		delete vs_;
		vs_ = nullptr;
	}

	if (gs_) {
		delete gs_;
		gs_ = nullptr;
	}

	if (fs_) {
		delete fs_;
		fs_ = nullptr;
	}

	// Delete if the address exists
	if (address_ > 0) {
		glDeleteProgram(address_);
	}
}
