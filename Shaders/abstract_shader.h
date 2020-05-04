#ifndef SHADERS_ABSRACT_SHADER_H
#define SHADERS_ABSRACT_SHADER_H

// Includes from the project
#include "DataStructures/byte_array.h"

// Includes from the STD
#include <string>

class ShaderException : public std::runtime_error
{
public:
	ShaderException(std::string err_message);
	const char* what() const noexcept;

private:
	std::string message_;
};

class AbstractShader
{
public:

	// Expensive copy here
	AbstractShader(const ByteArray&);
	AbstractShader(ByteArray&&);

	bool compile();
	unsigned int get_address() const;
	std::string get_compilation_message() const;

private:
	unsigned int address_;
	ByteArray data_;
	

	virtual int _get_shader_type() const = 0;
};

#endif //SHADERS_ABSRACT_SHADER_H
