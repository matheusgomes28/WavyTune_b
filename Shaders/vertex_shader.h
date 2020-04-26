#ifndef SHADERS_VERTEX_SHADER_H
#define SHADERS_VERTEX_SHADER_H

// Includes from this project
#include "abstract_shader.h"

// Includes from third party
#include <GL/glew.h>

class VertexShader : public AbstractShader
{
public:
	VertexShader(const std::string& path) : AbstractShader(path)
	{}

	// Only function to override
	int getGlEnum() const override
	{
		return GL_VERTEX_SHADER;
	}
};
#endif // SHADERS_VERTEX_SHADER_H
