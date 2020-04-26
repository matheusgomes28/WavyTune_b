#ifndef SHADERS_GEOMETRY_SHADER_H
#define SHADERS_GEOMETRY_SHADER_H

// Includes from this project
#include "abstract_shader.h"

// Includes from third party libs
#include <gl/glew.h>

class GeometryShader : public AbstractShader
{
public:
	GeometryShader(const std::string& path) : AbstractShader(path)
	{}

	int getGlEnum() const override
	{
		return GL_GEOMETRY_SHADER;
	}
};

#endif // SHADERS_GEOMETRY_SHADER_H
