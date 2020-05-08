#ifndef RENDERER_CONCRETE_RENDERER_H
#define RENDERER_CONCRETE_RENDERER_H

// Includes from OpenGL
#include "abstract_renderer.h"

// Includes from the std
#include <map>
#include <memory>
#include <stdexcept>
#include <string>


class VAO;
class VBO;
class Entity;
class DrawBuffer;
class ShaderProgram;

enum class BUFFER_TYPE { VERTEX, NORMAL, TEXTURE, COLOUR };



// Maybe bring this to another file, modularise
class ConcreteRendererException : public std::runtime_error
{
public:
	ConcreteRendererException(const std::string& message)
		: std::runtime_error{message}
	{
	}
};

class VertexBufferNotFoundException : public ConcreteRendererException
{
public:
	VertexBufferNotFoundException()
		: ConcreteRendererException("vertex buffer was not found")
	{
	}
};

class NormalBufferNotFoundException : public ConcreteRendererException
{
public:
	NormalBufferNotFoundException()
		: ConcreteRendererException("normal buffer was not found")
	{
	}
};

class ColourBufferNotFoundException : public ConcreteRendererException
{
public:
	ColourBufferNotFoundException()
		: ConcreteRendererException("colour buffer was not found")
	{
	}
};


class ShaderProgramNotSetException : public ConcreteRendererException
{
public:
	ShaderProgramNotSetException()
		: ConcreteRendererException("shader program not set")
	{
	}
};


class ConcreteRenderer : public AbstractRenderer
{
	using EntityDataMap = std::map<EntityPtr, std::vector<DrawBufferPtr>>;

	using VAOPtr = std::unique_ptr<VAO>;
	using VBOPtr = std::unique_ptr<VBO>;
 
public:
	ConcreteRenderer();
	~ConcreteRenderer();

	//! overrides
	void render(const glm::mat4& proj, const glm::mat4& view) override;
	void send_gpu_data() override;
	void add_entity_data(EntityPtr entPtr, DrawBufferPtr dataPtr) override;


protected:
	EntityDataMap entity_data_;
	VAOPtr vao_;
	std::map<BUFFER_TYPE, VBOPtr> vbos_;

	void set_shader(std::unique_ptr<ShaderProgram> shader_ptr) override;
	ShaderProgram* get_shader() const override;

private:

	// TODO : Could do function that uses the buffer
	// type to return the correct amount of data
	unsigned getVertexMemoryNeeded() const;
	unsigned getNormalMemoryNeeded() const;
	unsigned getColourMemoryNeeded() const;
	// TODO : Think of a smart way of doing the
	// stuff below
	//unsigned getMemoryNeeded(const BUFFER_TYPE& bt) const;

	std::unique_ptr<ShaderProgram> shader_program_;
	unsigned points_to_draw_;

	void allocateGPUMemory();
	void populateBuffers();
	void setUpVertexBufferAttributes();
	void setUpNormalBufferAttributes();
	void setUpColourBufferAttributes();
	void enableBuffers();
	void disableBuffers();


	inline VBO* _get_vertex_vbo();
	inline VBO* _get_normal_vbo();
	inline VBO* _get_colour_vbo();
};
#endif