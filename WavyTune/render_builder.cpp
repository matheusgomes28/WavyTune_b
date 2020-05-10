#include "render_builder.h"
#include "GLAbstractions/vao.h"
#include "GLAbstractions/vbo.h"
#include "GLAbstractions/vertex_attribute.h"
#include "Graphics/draw_data3.h"
#include "Graphics/draw_data4.h"
#include "Graphics/draw_buffer.h"
#include "Graphics/entity.h"

#include "Renderer/concrete_renderer.h"

#include "Shaders/vertex_shader.h"
#include "Shaders/fragment_shader.h"
#include "Shaders/geometry_shader.h"
#include "Shaders/shader_program.h"
#include "Shaders/vs.glsl.h"
#include "Shaders/fs.glsl.h"

template<class T, size_t N>
constexpr size_t size(T(&)[N])
{
	return N;
}

std::unique_ptr<AbstractRenderer> RenderBuilder::buildBarRenderer()
{
	auto retVal = std::make_unique<ConcreteRenderer>();

	// Create the entity needed and the data needed

	auto bar_buffer = std::make_unique<DrawBuffer>();

	std::vector<glm::vec3> vertex_array = {

		// Plane 1
		{-0.5, -0.5, -0.5},
		{ 0.5, -0.5, -0.5},
		{-0.5,  0.5, -0.5},
		{ 0.5, -0.5, -0.5},
		{ 0.5,  0.5, -0.5},
		{-0.5,  0.5, -0.5},

		// Plane 2
		{-0.5, -0.5,  0.5},
		{ 0.5, -0.5,  0.5},
		{-0.5,  0.5,  0.5},
		{ 0.5, -0.5,  0.5},
		{ 0.5,  0.5,  0.5},
		{-0.5,  0.5,  0.5},


		// Plane 3
		{-0.5, -0.5, -0.5},
		{-0.5, -0.5,  0.5},
		{-0.5,  0.5, -0.5},
		{-0.5, -0.5,  0.5},
		{-0.5,  0.5,  0.5},
		{-0.5,  0.5, -0.5},


		// Plane 4
		{ 0.5, -0.5, -0.5},
		{ 0.5, -0.5,  0.5},
		{ 0.5,  0.5, -0.5},
		{ 0.5, -0.5,  0.5},
		{ 0.5,  0.5,  0.5},
		{ 0.5,  0.5, -0.5},

		// Plane 5
		{-0.5,  0.5, -0.5},
		{ 0.5,  0.5, -0.5},
		{-0.5,  0.5,  0.5},
		{-0.5,  0.5,  0.5},
		{ 0.5,  0.5, -0.5},
		{ 0.5,  0.5,  0.5},

		// Plane 6
		{-0.5, -0.5, -0.5},
		{ 0.5, -0.5, -0.5},
		{-0.5, -0.5,  0.5},
		{-0.5, -0.5,  0.5},
		{ 0.5, -0.5, -0.5},
		{ 0.5, -0.5,  0.5}
	};
	auto vertices = std::make_unique<DrawData3>();
	vertices->setData(vertex_array);
	bar_buffer->set_vertices(std::move(vertices));

	std::vector<glm::vec3> normal_array = {
		// Plane 1
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		// Plane 2
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		// Plane 3
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		// Plane 4
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		// Plane 5
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		// Plane 6
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
	};
	auto normals = std::make_unique<DrawData3>();
	normals->setData(normal_array);
	bar_buffer->set_normals(std::move(normals));

	std::vector<glm::vec4> colour_array{
		// Plane 1
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},

		// Plane 2
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},

		// Plane 3
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},

		// Plane 4
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 1},

		// Plane 5
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0, 0, 1},

		// Plane 6
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 1, 1}
	};
	auto colours = std::make_unique<DrawData4>();
	colours->setData(colour_array);
	bar_buffer->set_colours(std::move(colours));


	auto bar_top_entity = std::make_shared<Entity>();
	bar_top_entity->setName("Bar entity");
	retVal->add_entity_data(bar_top_entity, std::move(bar_buffer));


	// Create the shader stuff here
	// TODO : Turn these into a builder
	ByteArray vs_data{size(vs)};
	vs_data.set_data(vs, size(vs));
	auto vertex_shader = std::make_unique<VertexShader>(std::move(vs_data));

	ByteArray fs_data{size(fs)};
	fs_data.set_data(fs, size(fs));
	auto fragment_shader = std::make_unique<FragmentShader>(std::move(fs_data));
	
	auto shader_program = std::make_unique<ShaderProgram>();
	shader_program->set_vertex_shader(std::move(vertex_shader));
	shader_program->set_fragment_shader(std::move(fragment_shader));
	shader_program->compile_and_link();
	retVal->set_shader(std::move(shader_program));
	return retVal;
}
