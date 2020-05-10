// The pre-compiled header
#include "pch.h"

// Includes from this project
#include "abstract_shader.h"

// Inclues from third party
#include <GL/glew.h>

AbstractShader::AbstractShader(const ByteArray& data)
	: data_{ data },
	address_{ 0 }
{
}

AbstractShader::AbstractShader(ByteArray&& data)
	: data_{ std::move(data) },
	address_{ 0 }
{
}


ShaderException::ShaderException(std::string err)
	: std::runtime_error{err},
	message_{ std::move(err) }
{
}

const char* ShaderException::what() const noexcept
{
	return message_.c_str();
}

bool AbstractShader::compile()
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

unsigned int AbstractShader::get_address() const
{
	return address_;
}

std::string AbstractShader::get_compilation_message() const
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
		else
		{
			return "compilation successful";
		}
	}
	else
	{
		return "shader has not been compiled yet";
	}
}