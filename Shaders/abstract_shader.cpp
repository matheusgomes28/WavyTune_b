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

AbstractShaderB::AbstractShaderB(const ByteArray& data)
	: data_{ data },
	address_{ 0 }
{
}

AbstractShaderB::AbstractShaderB(ByteArray&& data)
	: data_{ std::move(data) },
	address_{ 0 }
{
}

ShaderException::ShaderException(std::string err)
	: std::runtime_error{err},
	message_{ std::move(err) }
{
}

const char* ShaderException::what() const
{
	return message_.c_str();
}

bool AbstractShaderB::compile()
{
	// Check if this shader has already been
	// allocated in the GPU, if so, delete it
	if (address_ != 0)
	{
		glDeleteShader(address_);
	}

	address_ = glCreateShader(_get_shader_type());
	if (address_ != 0)
	{
		auto str_data = reinterpret_cast<const char*>(data_.get_data());
		int data_size = data_.get_size();
		glShaderSource(address_, 1, &str_data, &data_size);
		glCompileShader(address_);
	}
	else
	{
		throw ShaderException("could not allocate a shader");
	}
}

unsigned int AbstractShaderB::get_address() const
{
	return address_;
}

std::string AbstractShaderB::get_compilation_message() const
{
	if (address_ != 0)
	{
		constexpr unsigned int buffer_size = 512;
		char message_buffer[buffer_size];
		
		int retrieve_status;
		glGetShaderiv(address_, GL_COMPILE_STATUS, &retrieve_status);
		if (!retrieve_status)
		{
			glGetShaderInfoLog(address_, buffer_size, NULL, message_buffer);
			return message_buffer; // Should be converted to str
		}
	}
	else
	{
		return "shader has not been compiled yet";
	}
}