#ifndef SHADERS_ABSRACT_SHADER_H
#define SHADERS_ABSRACT_SHADER_H

// Includes from the project
#include "DataStructures/byte_array.h"

// Includes from the STD
#include <string>

// TODO : Make sure this is all done correctly with inheritance
// and abstraction.
class TextReader;
class AbstractShader
{
public:
	AbstractShader(const std::string& path);
	AbstractShader(const AbstractShader& fromCopy);
	AbstractShader(AbstractShader&& fromMover);
	~AbstractShader();

	void compile();
	unsigned int getAddress() const;
	std::string getCompileMessage() const;

	AbstractShader& operator=(const AbstractShader& fromcopy);
	AbstractShader&& operator=(AbstractShader&& fromMove);


private:
	std::string path;
	unsigned int address;
	TextReader* reader;

	unsigned int generateBuffer() const;
	std::string loadShaderData() const;
	virtual int getGlEnum() const = 0;

};


class ShaderException : public std::runtime_error
{
public:
	ShaderException(std::string err_message);
	const char* what() const noexcept;

private:
	std::string message_;
};
class AbstractShaderB
{
public:

	// Expensive copy here
	AbstractShaderB(const ByteArray&);
	AbstractShaderB(ByteArray&&);

	bool compile();
	unsigned int get_address() const;
	std::string get_compilation_message() const;

private:
	unsigned int address_;
	ByteArray data_;
	

	unsigned int _get_shader_type();
};

#endif //SHADERS_ABSRACT_SHADER_H
