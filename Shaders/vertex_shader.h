#ifndef SHADERS_VERTEX_SHADER_H
#define SHADERS_VERTEX_SHADER_H

// Includes from this project
#include "abstract_shader.h"

// Includes from third party
#include <GL/glew.h>

class VertexShader : public AbstractShader
{
public:
	VertexShader(const ByteArray& b) 
		: AbstractShader(b)
	{
	}

	VertexShader(ByteArray&& b)
		: AbstractShader(std::forward<ByteArray>(b))
	{
	}

private:
	int _get_shader_type() const override
	{
		return GL_VERTEX_SHADER;
	}
};
#endif // SHADERS_VERTEX_SHADER_H
