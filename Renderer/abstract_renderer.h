#ifndef RENDERERS_ABSTRACT_RENDERER_H
#define RENDERERS_ABSTRACT_RENDERER_H

// Includes from the STD
#include <vector>
#include <map>

// Includes from third party libs
#include <glm/glm.hpp>

class ShaderProgram;
class DrawBuffer;
class Entity;


//! TODO : Problem! May have different types of data to
//! render in a single render. Possible solutions are:

//!  - Inheritance: Make render type which contains renderers
//!    as instances, each renderer will deal with a type of data.
//!    Disadvantage is to make a different data for each type.

//!  - Use of different shaders and enums to figure out what 
//!    type of data to render. Disadvantage is that the renderer
//!    representation would have to be changed.




//! Class that defines the interface of a render.
//! Note that all the renderes in this program 
//! will inherit from this class, hence try and
//! keep it fairly tidy.
class AbstractRenderer
{
public:

	virtual void render(const glm::mat4& proj, const glm::mat4& view) = 0;
	virtual void sendGPUData() = 0;
	virtual void createGPUBuffers() = 0;
	virtual void addEntityData(Entity* ent, DrawBuffer* data) = 0;

protected:
	virtual void setShader() = 0;
	virtual ShaderProgram* getShader() const = 0;
};
#endif

