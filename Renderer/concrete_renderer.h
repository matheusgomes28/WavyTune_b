#ifndef RENDERER_CONCRETE_RENDERER_H
#define RENDERER_CONCRETE_RENDERER_H

// Includes from OpenGL
#include "abstract_renderer.h"

// Includes from the std
#include <map>
#include <exception>


class VAO;
class VBO;
class Entity;
class DrawBuffer;
class ShaderProgram;

enum class BUFFER_TYPE { VERTEX, NORMAL, TEXTURE, COLOUR };



// Maybe bring this to another file, modularise
class ConcreteRendererException : public std::exception
{
public:
	virtual const char* what() const override
	{
		return "Default concrete renderer error.";
	}
};

class BufferNotFoundException : public ConcreteRendererException
{
public:
	virtual const char* what() const override
	{
		return "Buffer was not found in memory.";
	}
};

class VertexBufferNotFound : public BufferNotFoundException
{
public:
	virtual const char* what() const override
	{
		return "Vertex buffer was not found.";
	}
};

class NormalBufferNotFound : public BufferNotFoundException
{
public:
	virtual const char* what() const override
	{
		return "Normal buffer was not found.";
	}
};

class ColourBufferNotFound : public BufferNotFoundException
{
public:
	virtual const char* what() const override
	{
		return "Colour buffer was not found.";
	}
};


class ConcreteRenderer : public AbstractRenderer
{
	using EntityDataMap = std::map<Entity*, std::vector<DrawBuffer*>>;

public:
	ConcreteRenderer();
	~ConcreteRenderer();

	//! overrides
	void render(const glm::mat4& proj, const glm::mat4& view) override;
	void sendGPUData() override;
	void createGPUBuffers() override;
	void addEntityData(Entity* entPtr, DrawBuffer* dataPtr) override;


protected:
	EntityDataMap entityData_;
	VAO* vao_;
	std::map<BUFFER_TYPE, VBO*> vbos_;

	void setShader() override;
	ShaderProgram* getShader() const override;

private:

	// TODO : Could do function that uses the buffer
	// type to return the correct amount of data
	unsigned getVertexMemoryNeeded() const;
	unsigned getNormalMemoryNeeded() const;
	unsigned getColourMemoryNeeded() const;
	// TODO : Think of a smart way of doing the
	// stuff below
	//unsigned getMemoryNeeded(const BUFFER_TYPE& bt) const;

	ShaderProgram* shaderProgram_;
	unsigned pointsToDraw_;

	void allocateGPUMemory();
	void populateBuffers();
	void setUpVertexBufferAttributes();
	void setUpNormalBufferAttributes();
	void setUpColourBufferAttributes();
	void enableBuffers();
	void disableBuffers();


	VBO* getVertexVBO();
	VBO* getNormalVBO();
	VBO* getColourVBO();
};
#endif