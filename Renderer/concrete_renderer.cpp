// Includes from this project
#include "concrete_renderer.h"

// Includes from WavyTune other libs
#include "GLAbstractions/vao.h"
#include "GLAbstractions/vbo.h"
#include "GLAbstractions/vertex_attribute.h"
#include "Graphics/colour_data.h"
#include "Graphics/draw_data3.h"
#include "Graphics/draw_buffer.h"
#include "Shaders/shader_program.h"

// Includes from third party
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

// Includes from the STD
#include <cmath>


std::vector<std::string> getError()
{
	std::vector<std::string> retVal;
	GLenum error = glGetError();
	while (error = glGetError() != GL_NO_ERROR) {

		switch (error)
		{
		case GL_INVALID_ENUM:
			retVal.push_back("Invalid enum.");
			break;
		case GL_INVALID_VALUE:
			retVal.push_back("Invalid function value.");
			break;
		case GL_INVALID_OPERATION:
			retVal.push_back("Invalid operation.");
			break;
		case GL_STACK_OVERFLOW:
			retVal.push_back("stack overflow.");
			break;
		case GL_STACK_UNDERFLOW:
			retVal.push_back("stack underflow.");
			break;
		case GL_OUT_OF_MEMORY:
			retVal.push_back("Out of memory.");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			retVal.push_back("Invalid framebuffer operation.");
			break;
		case GL_TABLE_TOO_LARGE:
			retVal.push_back("Table is too large.");
			break;
		}
	}
	return retVal;
}


ConcreteRenderer::ConcreteRenderer()
	: shader_program_(nullptr),
	vao_(std::make_unique<VAO>()),
	points_to_draw_{ 0 }
{
	// insert the stuff in vbos
	auto vertex_vbo = std::make_unique<VBO>();
	vbos_.insert({BUFFER_TYPE::VERTEX, std::move(vertex_vbo)});
	auto normal_vbo = std::make_unique<VBO>();
	vbos_.insert({BUFFER_TYPE::NORMAL, std::move(normal_vbo)});
	auto colour_vbo = std::make_unique<VBO>();
	vbos_.insert({BUFFER_TYPE::COLOUR,std::move(colour_vbo)});
}

ConcreteRenderer::~ConcreteRenderer()
{
	vbos_.clear();
	entity_data_.clear();
}

void ConcreteRenderer::set_shader(std::unique_ptr<ShaderProgram> shader_ptr)
{
	shader_program_ = std::move(shader_ptr);
}

ShaderProgram* ConcreteRenderer::get_shader() const
{
	return shader_program_.get();
}

VBO* ConcreteRenderer::_get_vertex_vbo()
{
	auto found = vbos_.find(BUFFER_TYPE::VERTEX);
	if (found != end(vbos_))
	{
		if (found->second) {
			return found->second.get();
		}
	}

	throw VertexBufferNotFoundException();
}

VBO* ConcreteRenderer::_get_normal_vbo()
{
	auto found = vbos_.find(BUFFER_TYPE::NORMAL);
	if (found != end(vbos_))
	{
		if (found->second)
		{
			return found->second.get();
		}
	}
	throw NormalBufferNotFoundException();
}

VBO* ConcreteRenderer::_get_colour_vbo()
{
	auto found = vbos_.find(BUFFER_TYPE::COLOUR);
	if (found != end(vbos_))
	{
		if (found->second)
		{
			return found->second.get();
		}
	}
	throw ColourBufferNotFoundException();
}


void ConcreteRenderer::send_gpu_data()
{
	points_to_draw_ = 0;
	get_shader()->use();
	glBindVertexArray(vao_->getId());

	// Allocate enough memory at the buffers
	allocateGPUMemory();
	populateBuffers();
	setUpVertexBufferAttributes();
	setUpNormalBufferAttributes();
	setUpColourBufferAttributes();

	auto errors = getError();
	get_shader()->unuse();
}

void ConcreteRenderer::allocateGPUMemory()
{
	//! Assumes we have already bound the 
	//! vertex array object
	VBO* vertexVBO = _get_vertex_vbo();
	vertexVBO->allocateMemory(getVertexMemoryNeeded());
	VBO* normalVBO = _get_normal_vbo();
	normalVBO->allocateMemory(getNormalMemoryNeeded());
	VBO* colourVBO = _get_colour_vbo();
	colourVBO->allocateMemory(getColourMemoryNeeded());
}

void ConcreteRenderer::populateBuffers()
{

	VBO* vertexVBO = _get_vertex_vbo();
	VBO* normalVBO = _get_normal_vbo();
	VBO* colourVBO = _get_colour_vbo();

	points_to_draw_= 0;
	unsigned vertexOffset = 0;
	unsigned normalOffset = 0;
	unsigned colourOffset = 0;
	for (auto& kvPair : entity_data_) {
		for (auto& buffer : kvPair.second) {
			vertexVBO->addData(buffer->getVertices().getData(), vertexOffset);
			vertexOffset += buffer->getVertices().getGPUSize();
			normalVBO->addData(buffer->getNormals().getData(), normalOffset);
			normalOffset += buffer->getNormals().getGPUSize();
			colourVBO->addData(buffer->getColours().getData(), colourOffset);
			colourOffset += buffer->getNormals().getGPUSize();

			points_to_draw_ += buffer->getVertices().getData().size();
		}
	}

	// TODO : figure out how the ssbos will be sent
	// for the entity transformations
}

void ConcreteRenderer::setUpVertexBufferAttributes()
{
	VBO* vertexVBO = _get_vertex_vbo();
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO->getId());

	VertexAttribute* vertexAttribs = new VertexAttribute;
	vertexAttribs->setOffset(0);
	vertexAttribs->setSize(3); // x,y,y
	vertexAttribs->setNormalised(false);
	vertexAttribs->setStride(0);
	vertexAttribs->setType(VERTEX_TYPE::FLOAT);
	vao_->addBufferConfigs(vertexVBO, vertexAttribs);

	// Get the layout location
	GLint posPtr = glGetAttribLocation(get_shader()->get_address(), "aPos");

	glVertexAttribPointer(
		posPtr,
		vertexAttribs->getSize(),
		(int)vertexAttribs->getType(),
		vertexAttribs->getNormalised(),
		vertexAttribs->getStride(),
		(GLvoid*)vertexAttribs->getOffset()
		);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ConcreteRenderer::setUpNormalBufferAttributes()
{
	VBO* normalVBO = _get_normal_vbo();
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO->getId());

	VertexAttribute* normalAttribs = new VertexAttribute;
	normalAttribs->setOffset(0);
	normalAttribs->setSize(3); // xn, yn, zn
	normalAttribs->setNormalised(false);
	normalAttribs->setStride(0);
	normalAttribs->setType(VERTEX_TYPE::FLOAT);
	vao_->addBufferConfigs(normalVBO, normalAttribs);

	// Get the layout location for normals
	GLint norPtr = glGetAttribLocation(get_shader()->get_address(), "aNor");

	glVertexAttribPointer(
		norPtr,
		normalAttribs->getSize(),
		(int)normalAttribs->getType(),
		normalAttribs->getNormalised(),
		normalAttribs->getStride(),
		(GLvoid*)normalAttribs->getOffset()
		);

	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

void ConcreteRenderer::setUpColourBufferAttributes()
{
	VBO* colourVBO = _get_colour_vbo();
	glBindBuffer(GL_ARRAY_BUFFER, colourVBO->getId());

	VertexAttribute* colourAttribute = new VertexAttribute;
	colourAttribute->setOffset(0);
	colourAttribute->setSize(4); // r, g, b, a
	colourAttribute->setNormalised(true); // Just in case its outside the range
	colourAttribute->setStride(0);
	colourAttribute->setType(VERTEX_TYPE::FLOAT);
	vao_->addBufferConfigs(colourVBO, colourAttribute);

	// Get the layout location
	GLint colPtr = glGetAttribLocation(get_shader()->get_address(), "aCol");

	glVertexAttribPointer(
		colPtr,
		colourAttribute->getSize(),
		(int)colourAttribute->getType(),
		colourAttribute->getNormalised(),
		colourAttribute->getStride(),
		(GLvoid*)colourAttribute->getOffset()
		);

	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

void ConcreteRenderer::render(const glm::mat4& proj, const glm::mat4& view)
{
	auto errors = getError();
	//! TODO : Implement this

	//! The last piece of the puzzle. Problem is to
	//! think about how to link the attributes and
	//! buffers in the VAO. Specifically, where is the
	//! best place to send the information to the GPU?
	//! Would it be here or in the VAO object itself?
	get_shader()->use();

	float green = abs(0.6 + sin(glfwGetTime() * 2) / 2.0);
	glm::mat4 rotate1 = glm::rotate((float)sin(glfwGetTime() * 2) * 3.14159f, glm::vec3(1, 0, 0));
	glm::mat4 rotate2 = glm::rotate((float)cos(glfwGetTime() * 2) * 3.14159f, glm::vec3(0, 1, 0));

	get_shader()->setUniform("green", green);
	get_shader()->setUniform("proj", proj);
	get_shader()->setUniform("view", view);
	get_shader()->setUniform("rotate", rotate1 * rotate2);


	// Enabling some features
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	enableBuffers();

	// Enable vertices and normals for drawing
	glBindVertexArray(vao_->getId());
	glDrawArrays(GL_TRIANGLES, 0, points_to_draw_);
	glBindVertexArray(0);


	disableBuffers();
	get_shader()->unuse();
	errors = getError();
}

void ConcreteRenderer::enableBuffers()
{
	// Get the layout locations
	GLuint vertexPtr = glGetAttribLocation(get_shader()->get_address(), "aPos");
	glEnableVertexAttribArray(vertexPtr);

	GLuint normalPtr = glGetAttribLocation(get_shader()->get_address(), "aNor");
	glEnableVertexAttribArray(normalPtr);

	GLuint colourPtr = glGetAttribLocation(get_shader()->get_address(), "aCol");
	glEnableVertexAttribArray(colourPtr);
}

void ConcreteRenderer::disableBuffers()
{
	// Get the layout locations
	GLuint vertexPtr = glGetAttribLocation(get_shader()->get_address(), "aPos");
	glDisableVertexAttribArray(vertexPtr);

	GLuint normalPtr = glGetAttribLocation(get_shader()->get_address(), "aNor");
	glDisableVertexAttribArray(normalPtr);

	GLuint colourPtr = glGetAttribLocation(get_shader()->get_address(), "aCol");
	glDisableVertexAttribArray(colourPtr);
}


void ConcreteRenderer::add_entity_data(EntityPtr entPtr, DrawBufferPtr buffer)
{
	auto found = entity_data_.find(entPtr);
	if (found != end(entity_data_)) {
		found->second.push_back( std::move(buffer) );
	}
	else {
		entity_data_[entPtr] = { std::move(buffer) };
	}
}


unsigned ConcreteRenderer::getVertexMemoryNeeded() const
{
	unsigned total = 0;
	for (auto& kvPair : entity_data_) {
		for (auto& dataPtr : kvPair.second) {
			total += dataPtr->getVertices().getGPUSize();
		}
	}
	return total;
}

unsigned ConcreteRenderer::getNormalMemoryNeeded() const
{
	unsigned total = 0;
	for (auto& kvPair : entity_data_) {
		for (auto& dataPtr : kvPair.second) {
			total += dataPtr->getNormals().getGPUSize();
		}
	}
	return total;
}

unsigned ConcreteRenderer::getColourMemoryNeeded() const
{
	unsigned total = 0;
	for (auto& kvPair : entity_data_) {
		for (auto& dataPtr : kvPair.second) {
			total += dataPtr->getColours().getGPUSize();
		}
	}
	return total;
}
