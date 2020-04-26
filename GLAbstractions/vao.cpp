#include "pch.h"
#include "vao.h"

VAO::VAO()
	: AbstractIdObject()
{
	generateId();
}

VAO::VAO(const std::string& name)
	: AbstractIdObject(name)
{
	generateId();
}

void VAO::generateId()
{
	GLuint newVAO = 0;
	glGenVertexArrays(1, &newVAO);
	setId(newVAO);
}

void VAO::addBufferConfigs(BufferPtr buffer, AttributesPtr attr)
{
	bufferSettings_[buffer] = attr;
}

const VAO::BufferSettingsMap& VAO::getBufferConfigs() const
{
	return bufferSettings_;
}
