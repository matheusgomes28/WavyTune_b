#ifndef RENDERERS_ABSTRACT_RENDERER_H
#define RENDERERS_ABSTRACT_RENDERER_H

// Includes from the STD
#include <map>
#include <memory>
#include <vector>

// Includes from third party libs
#include <glm/glm.hpp>

class ShaderProgram;
class DrawBuffer;
class Entity;

//! Class that defines the interface of a render.
//! Note that all the renderes in this program 
//! will inherit from this class, hence try and
//! keep it fairly tidy.
class AbstractRenderer
{
public:
	using EntityPtr = std::shared_ptr<Entity>;
	using DrawBufferPtr = std::unique_ptr<DrawBuffer>;

	virtual void render(const glm::mat4& proj, const glm::mat4& view) = 0;
	virtual void send_gpu_data() = 0;
	virtual void add_entity_data(EntityPtr ent, DrawBufferPtr data) = 0;
	virtual void set_shader(std::unique_ptr<ShaderProgram> shader_ptr) = 0;

protected:
	virtual ShaderProgram* get_shader() const = 0;
};
#endif

