#ifndef SHADERS_FRAGMENT_SHADER_H
#define SHADERS_FRAGMENT_SHADER_H
// Includes from this project
#include "abstract_shader.h"

// Includes from third party
#include <gl/glew.h>


class FragmentShader : public AbstractShader
{
public:
	FragmentShader(const std::string& path) : AbstractShader(path)
	{}

	int getGlEnum() const override
	{
		return GL_FRAGMENT_SHADER;
	}
};
#endif
