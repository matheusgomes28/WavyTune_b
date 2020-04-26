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


#define VS_PATH "C:\\Users\\MGOMES1\\Desktop\\audioVisualiser\\vs.glsl"
#define FS_PATH "C:\\Users\\MGOMES1\\Desktop\\audioVisualiser\\fs.glsl"

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
	: shaderProgram_(nullptr),
	vao_(nullptr),
	pointsToDraw_{ 0 }
{
	setShader();
}

ConcreteRenderer::~ConcreteRenderer()
{
	if (shaderProgram_) {
		delete shaderProgram_;
		shaderProgram_ = nullptr;
	}

	// Delete each vao and clear vec
	if (vao_) delete vao_;

	// Delete the buffers this renderer owns
	for (auto kvPair : vbos_) {
		if (kvPair.second) {
			delete kvPair.second;
		}
	}
	vbos_.clear();

	// Delete the entity data
	for (auto kvPair : entityData_) {

		// Delete the entity ptr
		if (kvPair.first) delete kvPair.first;

		for (DrawBuffer* buffer : kvPair.second) {
			if (buffer) delete buffer;
		}
	}
	entityData_.clear();
}

void ConcreteRenderer::setShader()
{
	auto errors = getError();
	// Make sure we delete it if its 
	// already created
	if (shaderProgram_) {
		delete shaderProgram_;
		shaderProgram_ = nullptr;
	}

	// TODO : Create the shader objects here
	// TODO : Remove hard-coded path for the shaders
	shaderProgram_ = new ShaderProgram(VS_PATH, FS_PATH);
	shaderProgram_->compileAndLink();
	errors = getError();
}

ShaderProgram* ConcreteRenderer::getShader() const
{
	return shaderProgram_;
}

void ConcreteRenderer::createGPUBuffers()
{
	auto errors = getError();
	// Make sure these are smart pointers
	vao_ = new VAO{};

	VBO* vertexVBO = new VBO{};
	vbos_.insert({ BUFFER_TYPE::VERTEX, vertexVBO });

	VBO* normalVBO = new VBO{};
	vbos_.insert({ BUFFER_TYPE::NORMAL, normalVBO });

	VBO* colourVBO = new VBO{};
	vbos_.insert({ BUFFER_TYPE::COLOUR, colourVBO });
}

VBO* ConcreteRenderer::getVertexVBO()
{
	auto found = vbos_.find(BUFFER_TYPE::VERTEX);
	if (found != end(vbos_)) {
		if (found->second) {
			return found->second;
		}
		else {
			vbos_.erase(BUFFER_TYPE::VERTEX);
		}
	}

	throw VertexBufferNotFound();
}

VBO* ConcreteRenderer::getNormalVBO()
{
	auto found = vbos_.find(BUFFER_TYPE::NORMAL);
	if (found != end(vbos_)) {
		if (found->second) {
			return found->second;
		}
		else {
			vbos_.erase(BUFFER_TYPE::NORMAL);
		}
	}

	throw NormalBufferNotFound();
}

VBO* ConcreteRenderer::getColourVBO()
{
	auto found = vbos_.find(BUFFER_TYPE::COLOUR);
	if (found != end(vbos_)) {
		if (found->second) {
			return found->second;
		}
		else {
			vbos_.erase(BUFFER_TYPE::COLOUR);
		}
	}

	throw ColourBufferNotFound();
}


void ConcreteRenderer::sendGPUData()
{
	pointsToDraw_ = 0;
	getShader()->use();
	glBindVertexArray(vao_->getId());

	// Allocate enough memory at the buffers
	allocateGPUMemory();
	populateBuffers();
	setUpVertexBufferAttributes();
	setUpNormalBufferAttributes();
	setUpColourBufferAttributes();

	auto errors = getError();
	getShader()->unuse();
}

void ConcreteRenderer::allocateGPUMemory()
{
	//! Assumes we have already bound the 
	//! vertex array object
	VBO* vertexVBO = getVertexVBO();
	vertexVBO->allocateMemory(getVertexMemoryNeeded());
	VBO* normalVBO = getNormalVBO();
	normalVBO->allocateMemory(getNormalMemoryNeeded());
	VBO* colourVBO = getColourVBO();
	colourVBO->allocateMemory(getColourMemoryNeeded());
}

void ConcreteRenderer::populateBuffers()
{

	VBO* vertexVBO = getVertexVBO();
	VBO* normalVBO = getNormalVBO();
	VBO* colourVBO = getColourVBO();

	pointsToDraw_ = 0;
	unsigned vertexOffset = 0;
	unsigned normalOffset = 0;
	unsigned colourOffset = 0;
	for (auto& kvPair : entityData_) {
		for (DrawBuffer* buffer : kvPair.second) {
			vertexVBO->addData(buffer->getVertices().getData(), vertexOffset);
			vertexOffset += buffer->getVertices().getGPUSize();
			normalVBO->addData(buffer->getNormals().getData(), normalOffset);
			normalOffset += buffer->getNormals().getGPUSize();
			colourVBO->addData(buffer->getColours().getData(), colourOffset);
			colourOffset += buffer->getNormals().getGPUSize();

			pointsToDraw_ += buffer->getVertices().getData().size();
		}
	}

	// TODO : figure out how the ssbos will be sent
	// for the entity transformations
}

void ConcreteRenderer::setUpVertexBufferAttributes()
{
	VBO* vertexVBO = getVertexVBO();
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO->getId());

	VertexAttribute* vertexAttribs = new VertexAttribute;
	vertexAttribs->setOffset(0);
	vertexAttribs->setSize(3); // x,y,y
	vertexAttribs->setNormalised(false);
	vertexAttribs->setStride(0);
	vertexAttribs->setType(VERTEX_TYPE::FLOAT);
	vao_->addBufferConfigs(vertexVBO, vertexAttribs);

	// Get the layout location
	GLint posPtr = glGetAttribLocation(getShader()->getAddress(), "aPos");

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
	VBO* normalVBO = getNormalVBO();
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO->getId());

	VertexAttribute* normalAttribs = new VertexAttribute;
	normalAttribs->setOffset(0);
	normalAttribs->setSize(3); // xn, yn, zn
	normalAttribs->setNormalised(false);
	normalAttribs->setStride(0);
	normalAttribs->setType(VERTEX_TYPE::FLOAT);
	vao_->addBufferConfigs(normalVBO, normalAttribs);

	// Get the layout location for normals
	GLint norPtr = glGetAttribLocation(getShader()->getAddress(), "aNor");

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
	VBO* colourVBO = getColourVBO();
	glBindBuffer(GL_ARRAY_BUFFER, colourVBO->getId());

	VertexAttribute* colourAttribute = new VertexAttribute;
	colourAttribute->setOffset(0);
	colourAttribute->setSize(4); // r, g, b, a
	colourAttribute->setNormalised(true); // Just in case its outside the range
	colourAttribute->setStride(0);
	colourAttribute->setType(VERTEX_TYPE::FLOAT);
	vao_->addBufferConfigs(colourVBO, colourAttribute);

	// Get the layout location
	GLint colPtr = glGetAttribLocation(getShader()->getAddress(), "aCol");

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
	getShader()->use();

	float green = abs(0.6 + sin(glfwGetTime() * 2) / 2.0);
	glm::mat4 rotate1 = glm::rotate((float)sin(glfwGetTime() * 2) * 3.14159f, glm::vec3(1, 0, 0));
	glm::mat4 rotate2 = glm::rotate((float)cos(glfwGetTime() * 2) * 3.14159f, glm::vec3(0, 1, 0));

	getShader()->setUniform("green", green);
	getShader()->setUniform("proj", proj);
	getShader()->setUniform("view", view);
	getShader()->setUniform("rotate", rotate1 * rotate2);


	// Enabling some features
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	enableBuffers();

	// Enable vertices and normals for drawing
	glBindVertexArray(vao_->getId());
	glDrawArrays(GL_TRIANGLES, 0, pointsToDraw_);
	glBindVertexArray(0);


	disableBuffers();
	getShader()->unuse();
	errors = getError();
}

void ConcreteRenderer::enableBuffers()
{
	// Get the layout locations
	GLuint vertexPtr = glGetAttribLocation(getShader()->getAddress(), "aPos");
	glEnableVertexAttribArray(vertexPtr);

	GLuint normalPtr = glGetAttribLocation(getShader()->getAddress(), "aNor");
	glEnableVertexAttribArray(normalPtr);

	GLuint colourPtr = glGetAttribLocation(getShader()->getAddress(), "aCol");
	glEnableVertexAttribArray(colourPtr);
}

void ConcreteRenderer::disableBuffers()
{
	// Get the layout locations
	GLuint vertexPtr = glGetAttribLocation(getShader()->getAddress(), "aPos");
	glDisableVertexAttribArray(vertexPtr);

	GLuint normalPtr = glGetAttribLocation(getShader()->getAddress(), "aNor");
	glDisableVertexAttribArray(normalPtr);

	GLuint colourPtr = glGetAttribLocation(getShader()->getAddress(), "aCol");
	glDisableVertexAttribArray(colourPtr);
}


void ConcreteRenderer::addEntityData(Entity* entPtr, DrawBuffer* buffer)
{
	auto found = entityData_.find(entPtr);
	if (found != end(entityData_)) {
		found->second.push_back(buffer);
	}
	else {
		entityData_[entPtr] = { buffer };
	}
}


unsigned ConcreteRenderer::getVertexMemoryNeeded() const
{
	unsigned total = 0;
	for (auto& kvPair : entityData_) {
		for (DrawBuffer* dataPtr : kvPair.second) {
			total += dataPtr->getVertices().getGPUSize();
		}
	}
	return total;
}

unsigned ConcreteRenderer::getNormalMemoryNeeded() const
{
	unsigned total = 0;
	for (auto& kvPair : entityData_) {
		for (DrawBuffer* dataPtr : kvPair.second) {
			total += dataPtr->getNormals().getGPUSize();
		}
	}
	return total;
}

unsigned ConcreteRenderer::getColourMemoryNeeded() const
{
	unsigned total = 0;
	for (auto& kvPair : entityData_) {
		for (DrawBuffer* dataPtr : kvPair.second) {
			total += dataPtr->getColours().getGPUSize();
		}
	}
	return total;
}
