// The pre-compiled header
#include "pch.h"

// Includes from this project
#include "abstract_shader.h"
#include "FileIO/text_reader.h"

// Inclues from third party
#include <GL/glew.h>

AbstractShader::AbstractShader(const std::string& path)
	: address(0u),
	path(path),
	reader(new TextReader(path))
{}

AbstractShader::AbstractShader(const AbstractShader& fromCopy)
	: address(0),
	path(fromCopy.path),
	reader(new TextReader(fromCopy.path))
{}

AbstractShader::AbstractShader(AbstractShader&& fromMove)
	: address(fromMove.address),
	path(fromMove.path),
	reader(fromMove.reader)
{}

AbstractShader::~AbstractShader()
{
	path = "";

	// Deete addres if it was allocated
	if (address != 0) {
		glDeleteShader(address);
	}

	// Delete the reader 
	if (reader) {
		delete reader;
	}
}


unsigned int AbstractShader::getAddress() const
{
	return address;
}

void AbstractShader::compile()
{
	address = generateBuffer();
	std::string data = loadShaderData();
	const int shaderSize = data.size();
	const char* c_str = data.c_str();

	glShaderSource(address, 1, &c_str, &shaderSize);
	glCompileShader(address);
}

std::string AbstractShader::getCompileMessage() const
{
	if (0 < address) {
		int success;

		// Buffer size for message
		const int BUFFER_SIZE = 512;
		char infoLog[BUFFER_SIZE];
		glGetShaderiv(address, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(address, BUFFER_SIZE, NULL, infoLog);
			return infoLog;
		}
		else {
			return "Successful compilation.";
		}

	}
	return "Not compiled yet.";
}

unsigned int AbstractShader::generateBuffer() const
{
	// Pretty starndard stuff
	return glCreateShader(getGlEnum());
}

std::string AbstractShader::loadShaderData() const
{
	reader->openFile();
	std::string strData = reader->readFile();
	reader->closeFile();
	return strData; // Should return the pointer to the underlying data
}

// Operators for copy and move
AbstractShader& AbstractShader::operator=(const AbstractShader& fromCopy)
{
	this->address = 0;
	this->path = fromCopy.path;
	this->reader = new TextReader(path);
	return *this;
}

AbstractShader&& AbstractShader::operator=(AbstractShader&& fromMove)
{
	this->address = fromMove.address;
	fromMove.address = 0;

	this->path = std::move(fromMove.path);

	this->reader = fromMove.reader;
	fromMove.reader = nullptr;

	return std::move(*this);
}